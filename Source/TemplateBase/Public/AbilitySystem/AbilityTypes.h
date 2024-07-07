// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "ScalableFloat.h"
#include "AbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageRange
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Meta")
	FScalableFloat DamageMin = FScalableFloat();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Meta")
	FScalableFloat DamageMax = FScalableFloat();
};

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY(BlueprintReadWrite) TObjectPtr<UObject> WorldContextObject = nullptr;
	UPROPERTY(BlueprintReadWrite) TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	UPROPERTY(BlueprintReadWrite) TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;
	UPROPERTY(BlueprintReadWrite) TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;
	UPROPERTY(BlueprintReadWrite) FDamageRange BaseDamageRange = FDamageRange();
	UPROPERTY(BlueprintReadWrite) float AbilityLevel = 1.f;
	UPROPERTY(BlueprintReadWrite) FGameplayTag DamageType = FGameplayTag();;
	UPROPERTY(BlueprintReadWrite) float StatusEffectChance = 0.f;
	UPROPERTY(BlueprintReadWrite) float StatusEffectDamage = 0.f;
	UPROPERTY(BlueprintReadWrite) float StatusEffectFrequency = 0.f;
	UPROPERTY(BlueprintReadWrite) float StatusEffectDuration = 0.f;
	UPROPERTY(BlueprintReadWrite) float DeathImpulseMagnitude = 0.f;
	UPROPERTY(BlueprintReadWrite) FVector DeathImpulse = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite) float AirborneForceMagnitude = 0.f;
	UPROPERTY(BlueprintReadWrite) float AirborneChance = 0.f;
	UPROPERTY(BlueprintReadWrite) FVector AirborneForce = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite) bool bIsRadialDamage = false;
	UPROPERTY(BlueprintReadWrite) float RadialDamageInnerRadius = 0.f;
	UPROPERTY(BlueprintReadWrite) float RadialDamageOuterRadius = 0.f;
	UPROPERTY(BlueprintReadWrite) FVector RadialDamageOrigin = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite) float ShowDamageDelay = 0.f;
	
};

USTRUCT(BlueprintType)
struct FBaseGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsSuccessfulStatusEffect() const { return bIsSuccessfulStatusEffect; }
	float GetStatusEffectDamage() const { return StatusEffectDamage; }
	float GetStatusEffectDuration() const { return StatusEffectDuration; }
	float GetStatusEffectFrequency() const { return StatusEffectFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetAirborneForce() const { return AirborneForce; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
	float GetShowDamageDelay() const { return ShowDamageDelay; }

	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsSuccessfulStatusEffect(bool bInIsSuccessful) { bIsSuccessfulStatusEffect = bInIsSuccessful; }
	void SetStatusEffectDamage(float InDamage) { StatusEffectDamage = InDamage; }
	void SetStatusEffectDuration(float InDuration) { StatusEffectDuration = InDuration; }
	void SetStatusEffectFrequency(float InFrequency) { StatusEffectFrequency = InFrequency; }
	void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType) { DamageType = InDamageType; };
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	void SetAirborneForce(const FVector& InAirborneForce) { AirborneForce = InAirborneForce; }
	void SetIsRadialDamage(bool bInIsRadialDamage )  { bIsRadialDamage = bInIsRadialDamage; }
	void SetRadialDamageInnerRadius(float InRadialInnerRadius)  { RadialDamageInnerRadius = InRadialInnerRadius; }
	void SetRadialDamageOuterRadius(float InRadialOuterRadius)  { RadialDamageOuterRadius = InRadialOuterRadius; }
	void SetRadialDamageOrigin(const FVector& InRadialOrigin)  { RadialDamageOrigin = InRadialOrigin; }
	void SetShowDamageDelay(float InShowDamageDelay)  { ShowDamageDelay = InShowDamageDelay; }
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
	
	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FGameplayEffectContext* Duplicate() const override
	{
		FBaseGameplayEffectContext* NewContext = new FBaseGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;
	
	UPROPERTY()
	bool bIsSuccessfulStatusEffect = false;
	
	UPROPERTY()
	float StatusEffectDamage = 0.f;
	
	UPROPERTY()
	float StatusEffectDuration = 0.f;
	
	UPROPERTY()
	float StatusEffectFrequency = 0.f;
	
	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;
	
	UPROPERTY()
	FVector AirborneForce = FVector::ZeroVector;
	
	UPROPERTY()
	bool bIsRadialDamage = false;
	
	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;
	
	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
	
	UPROPERTY()
	float ShowDamageDelay = 0.f;
};

template<>
struct TStructOpsTypeTraits<FBaseGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FBaseGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};