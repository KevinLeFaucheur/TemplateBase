// Retropsis @ 2024

#include "AbilitySystem/Ability/BeamAbility.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UBeamAbility::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if(HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UBeamAbility::StoreOwnerVariables()
{
	if(CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UBeamAbility::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (const USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);
			FHitResult HitResult;
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::ForDuration,
				HitResult,
				true);

			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}
	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		if(!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UBeamAbility::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UBeamAbility::PrimaryTargetDied);
		}
	}
}

void UBeamAbility::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	TArray<AActor*> OverlappingActors;
	UBaseAbilitySystemLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(), OverlappingActors, ActorsToIgnore, 850.f, MouseHitLocation);

	const int32 NumAdditionalTargets = FMath::Min(MaxNumHitTargets, GetAbilityLevel() - 1);
	// const int32 NumAdditionalTargets = 5;

	UBaseAbilitySystemLibrary::GetClosestTargets(NumAdditionalTargets, OverlappingActors, OutAdditionalTargets, MouseHitActor->GetActorLocation());

	for (AActor* Target : OutAdditionalTargets)
	{
		if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			if(!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UBeamAbility::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UBeamAbility::AdditionalTargetDied);
			}
		}
	}
}

FString UBeamAbility::GetDescription(int32 Level)
{
	const int32 MinDamage = DamageRange.DamageMin.GetValueAtLevel(Level);
	const int32 MaxDamage = DamageRange.DamageMax.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if(Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>ELECTROCUTE</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>MP: </><ManaCost>%.1f</>\n"
			"<Small>CD: </><Cooldown>%.1f</>\n\n"
			
			"<Default>Launches a lightning beam, dealing: </>"
			"<Damage>%d-%d</>"
			"<Default> lightning damage with a chance to stun.</>\n\n"
			), Level, ManaCost, Cooldown, MinDamage, MaxDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>ELECTROCUTE</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>MP: </><ManaCost>%.1f</>\n"
			"<Small>CD: </><Cooldown>%.1f</>\n\n"
			
			"<Default>Launches a lightning beam, propagating to %d additional targets and dealing: </>"
			"<Damage>%d-%d</>"
			"<Default> lightning damage with a chance to stun.</>\n\n"
			), Level, ManaCost, Cooldown, FMath::Min(Level, FMath::Min(MaxNumHitTargets, Level - 1)), MinDamage, MaxDamage);
	}
}

FString UBeamAbility::GetNextLevelDescription(int32 Level)
{
	const int32 MinDamage = DamageRange.DamageMin.GetValueAtLevel(Level);
	const int32 MaxDamage = DamageRange.DamageMax.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL:</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>MP: </><ManaCost>%.1f</>\n"
		"<Small>CD: </><Cooldown>%.1f</>\n\n"
			
		"<Default>Launches a lightning beam, propagating to %d additional targets and dealing: </>"
		"<Damage>%d-%d</>"
		"<Default> lightning damage with a chance to stun.</>\n\n"
		), Level, ManaCost, Cooldown, FMath::Min(Level, FMath::Min(MaxNumHitTargets, Level - 1)), MinDamage, MaxDamage);
}