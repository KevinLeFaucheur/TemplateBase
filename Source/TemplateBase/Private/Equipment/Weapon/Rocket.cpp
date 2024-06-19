// Retropsis @ 2024

#include "Equipment/Weapon/Rocket.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"

ARocket::ARocket()
{
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>("RocketMesh");
	RocketMesh->SetupAttachment(GetRootComponent());
	RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARocket::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// if(!IsValidOverlap(OtherActor)) return;
	//
	// if(HasAuthority())
	// {
	// 	if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	// 	{
	// 		DamageEffectParams.DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
	// 		DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
	// 		UBaseAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
	// 	}
	// }
	Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}
