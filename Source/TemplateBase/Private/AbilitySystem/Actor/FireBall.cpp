// Retropsis @ 2024

#include "AbilitySystem/Actor/FireBall.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"

void AFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AFireBall::OnHit()
{
	if(GetOwner())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FBaseGameplayTags::Get().GameplayCue_FireBlast, CueParams);
	}

	bHit = true;
}

void AFireBall::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsValidOverlap(OtherActor)) return;

	if(HasAuthority())
	{
		if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			DamageEffectParams.DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UBaseAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}
