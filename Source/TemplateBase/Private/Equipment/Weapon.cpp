// Retropsis @ 2024

#include "Equipment/Weapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"

AWeapon::AWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::ApplyHitScanDamage(AActor* TargetActor) const
{
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	FGameplayEffectSpecHandle DamageSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, SourceASC->MakeEffectContext());
	
	const float ScaledMagnitudeMin = DamageRange.DamageMin.GetValueAtLevel(1.f);
	const float ScaledMagnitudeMax = DamageRange.DamageMax.GetValueAtLevel(1.f);
	const float Magnitude = FMath::FRandRange(ScaledMagnitudeMin, ScaledMagnitudeMax);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, Magnitude);
	
	if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		TargetASC->ApplyGameplayEffectSpecToTarget( *DamageSpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	}
}

void AWeapon::CauseDamage(const FHitResult& Hit) const
{
	if (HasAuthority() && Hit.bBlockingHit)
	{
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, SourceASC->MakeEffectContext());
    
		const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
		
		const float ScaledMagnitudeMin = DamageRange.DamageMin.GetValueAtLevel(1.f);
		const float ScaledMagnitudeMax = DamageRange.DamageMax.GetValueAtLevel(1.f);
		const float Magnitude = FMath::FRandRange(ScaledMagnitudeMin, ScaledMagnitudeMax);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, Magnitude);
		
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Hit.GetActor()))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

FGameplayEffectSpecHandle AWeapon::MakeDamageEffectSpec() const
{
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, SourceASC->MakeEffectContext());

	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();

	const float ScaledMagnitudeMin = DamageRange.DamageMin.GetValueAtLevel(1.f);
	const float ScaledMagnitudeMax = DamageRange.DamageMax.GetValueAtLevel(1.f);
	const float Magnitude = FMath::FRandRange(ScaledMagnitudeMin, ScaledMagnitudeMax);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, Magnitude);

	return SpecHandle;
}

FDamageEffectParams AWeapon::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor, FVector InRadialDamageOrigin, bool bOverrideAirborneDirection,
	FVector AirborneDirectionOverride, bool bOverrideDeathImpulseDirection, FVector DeathImpulseDirectionOverride, bool bOverridePitch, float PitchOverride) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetOwner();
	Params.DamageEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamageRange.DamageMin = DamageRange.DamageMin.GetValueAtLevel(1.f);
	Params.BaseDamageRange.DamageMax = DamageRange.DamageMax.GetValueAtLevel(1.f);
	Params.AbilityLevel = 1.f;
	Params.DamageType = DamageType;
	Params.StatusEffectChance = /*StatusEffectChance*/ 0.f;
	Params.StatusEffectDamage = /*StatusEffectDamage*/ 0.f;
	Params.StatusEffectDuration = /*StatusEffectDuration*/ 0.f;
	Params.StatusEffectFrequency = /*StatusEffectFrequency*/ 0.f;
	Params.DeathImpulseMagnitude = /*DeathImpulseMagnitude*/ 0.f;
	Params.AirborneForceMagnitude = /*AirborneForceMagnitude*/ 0.f;
	Params.AirborneChance = /*AirborneChance*/ 0.f;

	// if(IsValid(TargetActor))
	// {
	// 	FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
	// 	if (bOverridePitch)
	// 	{
	// 		Rotation.Pitch = PitchOverride;
	// 	}
	// 	const FVector ToTarget = Rotation.Vector();
	// 	if(!bOverrideAirborneDirection) Params.AirborneForce = ToTarget * AirborneForceMagnitude;  
	// 	if(!bOverrideDeathImpulseDirection) Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
	// }
	// if(bOverrideAirborneDirection)
	// {
	// 	AirborneDirectionOverride.Normalize();
	// 	Params.AirborneForce = AirborneDirectionOverride * AirborneForceMagnitude;
	// 	if(bOverridePitch)
	// 	{
	// 		FRotator AirborneRotation = AirborneDirectionOverride.Rotation();
	// 		AirborneRotation.Pitch = PitchOverride;
	// 		Params.AirborneForce = AirborneRotation.Vector() * AirborneForceMagnitude;
	// 	}
	// }
	//
	// if(bOverrideDeathImpulseDirection)
	// {
	// 	DeathImpulseDirectionOverride.Normalize();
	// 	Params.DeathImpulse = DeathImpulseDirectionOverride * DeathImpulseMagnitude;
	// 	if(bOverridePitch)
	// 	{
	// 		FRotator DeathImpulseRotation = DeathImpulseDirectionOverride.Rotation();
	// 		DeathImpulseRotation.Pitch = PitchOverride;
	// 		Params.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
	// 	}
	// }

	if(bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageOrigin = InRadialDamageOrigin;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}

	return Params;
}

void AWeapon::PlayActiveAnimation()
{
	if(ActiveAnimation) Mesh->PlayAnimation(ActiveAnimation, false);
}

void AWeapon::DetachToolFromComponent()
{
	const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	Mesh->DetachFromComponent(DetachRules);
}

void AWeapon::OnEquipped()
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

void AWeapon::OnDropped()
{
	Super::OnDropped();
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AWeapon::OnSecondary()
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
