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

	UPROPERTY() TObjectPtr<UObject> WorldContextObject = nullptr;
	UPROPERTY() TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	UPROPERTY() TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;
	UPROPERTY() TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;
	UPROPERTY() FDamageRange BaseDamageRange = FDamageRange();
	// UPROPERTY() float BaseDamage = 0.f;
	UPROPERTY() float AbilityLevel = 1.f;
	UPROPERTY() FGameplayTag DamageType = FGameplayTag();;
	UPROPERTY() float StatusEffectChance = 0.f;
	UPROPERTY() float StatusEffectDamage = 0.f;
	UPROPERTY() float StatusEffectFrequency = 0.f;
	UPROPERTY() float StatusEffectDuration = 0.f;
};

USTRUCT(BlueprintType)
struct FBaseGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }

	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	
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