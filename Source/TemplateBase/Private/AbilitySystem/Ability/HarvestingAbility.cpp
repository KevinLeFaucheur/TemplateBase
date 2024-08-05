// Retropsis @ 2024

#include "AbilitySystem/Ability/HarvestingAbility.h"

#include "Equipment/Tool.h"
#include "Harvesting/TransientHarvestable.h"
#include "Interaction/EquipmentInterface.h"
#include "Interaction/HarvestableInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UHarvestingAbility::TryHarvest(const FRotator Rotation)
{
	ServerOnOverlap(Rotation);
}

void UHarvestingAbility::ServerOnOverlap_Implementation(const FRotator Rotation)
{
	if(GetAvatarActorFromActorInfo())
	{
		if(ATool* EquippedTool = IPlayerInterface::Execute_GetEquippedTool(GetAvatarActorFromActorInfo()))
		{
			HarvestingToolInfo = IEquipmentInterface::Execute_GetHarvestingToolInfo(EquippedTool);
		}
		
		FVector Start = IPlayerInterface::Execute_GetCameraLocation(GetAvatarActorFromActorInfo());
		const float DistanceToCharacter = (GetAvatarActorFromActorInfo()->GetActorLocation() - Start).Size();
		Start += UKismetMathLibrary::GetForwardVector(Rotation) * (DistanceToCharacter + 85.f);
		FVector End = Start + UKismetMathLibrary::GetForwardVector(Rotation) * HarvestingDistance;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
		FHitResult HitResult;
		UKismetSystemLibrary::SphereTraceSingle(this, Start, End, HarvestingRadius, TraceTypeQuery1, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

		if(HitResult.bBlockingHit)
		{
			if(HitResult.GetActor() && HitResult.GetActor()->Implements<UHarvestableInterface>())
			{
				Harvest(HarvestingToolInfo.HarvestingDamage, HitResult.GetActor());
			}
		}
	}
}

void UHarvestingAbility::Harvest(float Damage, AActor* HitActor)
{
	const float NewHealth = IHarvestableInterface::Execute_GetHealth(HitActor) - HarvestingToolInfo.HarvestingDamage;
	IHarvestableInterface::Execute_UpdateHealth(HitActor, NewHealth);
	const TObjectPtr<UHarvestableInfo> Resources = IHarvestableInterface::Execute_GetResources(HitActor).LoadSynchronous();
	CalculateResources(Resources->GivenResources, Damage);
	if(NewHealth <= 0.f)
	{
		ATransientHarvestable* TransientHarvestable = GetWorld()->SpawnActorDeferred<ATransientHarvestable>(
		Resources->TransientHarvestableClass.LoadSynchronous(),
		HitActor->GetTransform(), 
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		TransientHarvestable->Direction = GetAvatarActorFromActorInfo()->GetActorForwardVector();

		TransientHarvestable->FinishSpawning(HitActor->GetTransform());
		
		HitActor->Destroy();
	}
}

void UHarvestingAbility::CalculateResources(const TArray<FResource>& Resources, float Damage)
{
	for (const FResource& Resource : Resources)
	{
		// BaseVar * ServerRate * ToolTypeVar * ToolTierVar * Damage
		const float ServerRate = 1.f;
		const float ToolTypeValue =  Resource.PreferredTool == HarvestingToolInfo.HarvestingToolType ? FMath::FRandRange(.8f, 1.1f) : FMath::FRandRange(.04f, .5f);
		float ToolTierValue = 0.f;
		switch (HarvestingToolInfo.HarvestingToolTier) {
		case EHarvestingToolTier::Stone: ToolTierValue = FMath::FRandRange(.8f, 1.2f);
			break;
		case EHarvestingToolTier::Iron: ToolTierValue = FMath::FRandRange(1.2f, 1.8f);
			break;
		}
		const int32 NumResources = FMath::TruncToInt32(Resource.Quantity * ServerRate * ToolTypeValue * ToolTierValue * Damage); 
		if(NumResources > 0)
		{
			if(const UItemInfo* ResourceAsset = Resource.Resource.LoadSynchronous())
			{
				FInventoryItemData ResourceToAdd;
				ResourceToAdd.Asset =ResourceAsset ;
				ResourceToAdd.ID = ResourceAsset->ItemData.ID;
				ResourceToAdd.Quantity = NumResources;
				ResourceToAdd.Health = ResourceAsset->ItemData.Health;
				ResourceToAdd.MaxHealth = ResourceAsset->ItemData.MaxHealth;
				ResourceToAdd.StackSize = ResourceAsset->ItemData.StackSize;
				IPlayerInterface::Execute_AddHarvestedResources(GetAvatarActorFromActorInfo(), ResourceToAdd);
			}
		}
	}
}
