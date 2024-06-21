// Retropsis @ 2024

#include "Equipment/Weapon/MultiHitScanWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "Engine/SkeletalMeshSocket.h"

void AMultiHitScanWeapon::Activate(const FVector& HitTarget)
{
	ATool::Activate(HitTarget);
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();
	
	const USkeletalMeshSocket* MuzzleFlashSocket = GetMesh()->GetSocketByName(FName("MuzzleFlash"));
	if(MuzzleFlashSocket && InstigatorController)
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetMesh());
		const FVector Start = SocketTransform.GetLocation();

		TMap<UAbilitySystemComponent*, uint32> HitMap;
		for (int32 i = 0; i < NumHits; ++i)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(FireHit.GetActor());
			if(FireHit.GetActor()) GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Green, FString::Printf(TEXT("Hit: %s"), *FireHit.GetActor()->GetName()));
			if (HasAuthority() && TargetASC && UBaseAbilitySystemLibrary::IsHostile(OwnerPawn, FireHit.GetActor()))
			{
				if(HitMap.Contains(TargetASC))
				{
					HitMap[TargetASC]++;
				}
				else
				{
					HitMap.Emplace(TargetASC, 1);
				}
			}
			PlayHitEffects(FireHit, true);
		}

		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
		for (const TTuple<UAbilitySystemComponent*, unsigned> HitPair : HitMap)
		{
			if(SourceASC && HasAuthority())
			{
				const FGameplayEffectSpecHandle DamageSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, SourceASC->MakeEffectContext());
				const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	
				const float ScaledMagnitudeMin = DamageRange.DamageMin.GetValueAtLevel(1.f);
				const float ScaledMagnitudeMax = DamageRange.DamageMax.GetValueAtLevel(1.f);
				const float Magnitude = FMath::FRandRange(ScaledMagnitudeMin, ScaledMagnitudeMax);
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, Magnitude * HitPair.Value);
				
				if (HitPair.Key) // TODO: Probably don't need this check cause its checked earlier already
				{
					HitPair.Key->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
				}
			}
		}
	}
}
