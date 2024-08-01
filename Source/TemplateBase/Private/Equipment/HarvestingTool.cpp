// Retropsis @ 2024

#include "Equipment/HarvestingTool.h"

#include "Interaction/HarvestableInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/PlayerCharacter.h"

AHarvestingTool::AHarvestingTool()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ToolClass = EToolClass::HarvestingTool;

	bReplicates = true;

	InitializeTool();
}

void AHarvestingTool::DetachToolFromComponent()
{
	const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	Mesh->DetachFromComponent(DetachRules);
}

void AHarvestingTool::OnEquipped()
{
	Super::OnEquipped();
	Mesh->SetSimulatePhysics(false);
	Mesh->SetEnableGravity(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(bUsePhysicsAsset)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetEnableGravity(true);
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
}

void AHarvestingTool::OnDropped()
{
	Super::OnDropped();
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AHarvestingTool::OnSecondary()
{
	Super::OnSecondary();
	Mesh->SetSimulatePhysics(false);
	Mesh->SetEnableGravity(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(bUsePhysicsAsset)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetEnableGravity(true);
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
}

/*
 * Equipment Interface
 */
void AHarvestingTool::OnHarvestingNotify_Implementation()
{
	ClientGetViewRotation();
}

FHarvestingToolInfo AHarvestingTool::GetHarvestingToolInfo_Implementation()
{
	return FHarvestingToolInfo{ HarvestingToolType, HarvestingToolTier, HarvestingDamage };
}

void AHarvestingTool::ClientGetViewRotation_Implementation()
{
	ServerOnOverlap(IPlayerInterface::Execute_GetCameraRotation(GetOwner()));
}

void AHarvestingTool::ServerOnOverlap_Implementation(const FRotator Rotation)
{
	if(OwnerCharacter)
	{
		FVector Start = IPlayerInterface::Execute_GetCameraLocation(OwnerCharacter);
		const float DistanceToCharacter = (OwnerCharacter->GetActorLocation() - Start).Size();
		Start += UKismetMathLibrary::GetForwardVector(Rotation) * (DistanceToCharacter + 85.f);
		FVector End = Start + UKismetMathLibrary::GetForwardVector(Rotation) * HarvestingDistance;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(OwnerCharacter);
		FHitResult HitResult;
		UKismetSystemLibrary::SphereTraceSingle(this, Start, End, HarvestingRadius, TraceTypeQuery1, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

		if(HitResult.bBlockingHit)
		{
			if(HitResult.GetActor() && HitResult.GetActor()->Implements<UHarvestableInterface>())
			{
				Harvest(HarvestingDamage, HitResult.GetActor());
			}
		}
	}
}

void AHarvestingTool::Harvest(const float Damage, AActor* HitActor)
{
	const float NewHealth = IHarvestableInterface::Execute_GetHealth(HitActor) - HarvestingDamage;
	IHarvestableInterface::Execute_UpdateHealth(HitActor, NewHealth);
	const TObjectPtr<UHarvestableInfo> Resources = IHarvestableInterface::Execute_GetResources(HitActor).LoadSynchronous();
	CalculateResources(Resources->GivenResources, Damage);
	if(NewHealth <= 0.f)
	{
		// Destroy it
	}
}

void AHarvestingTool::CalculateResources(const TArray<FResource>& Resources, float Damage)
{
	for (const FResource& Resource : Resources)
	{
		// BaseVar * ServerRate * ToolTypeVar * ToolTierVar * Damage
		const float ServerRate = 1.f;
		const float ToolTypeValue =  Resource.PreferredTool == HarvestingToolType ? FMath::FRandRange(.8f, 1.1f) : FMath::FRandRange(.04f, .5f);
		float ToolTierValue = 0.f;
		switch (HarvestingToolTier) {
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
                IPlayerInterface::Execute_AddHarvestedResources(OwnerCharacter, ResourceToAdd);
			}
		}
	}
}
