// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAbilitySystemComponent;
class UNiagaraSystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTakeDamage, float /* DamageAmount */);

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEMPLATEBASE_API ICombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	int32 GetCharacterLevel();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeapon();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die(const FVector& DeathImpulse) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetImpactEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncrementMinionCount(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();

	virtual FOnASCRegistered& GetOnASCRegistered() = 0;
	virtual FOnDeath& GetOnDeathDelegate() = 0;
	virtual FOnTakeDamage& GetOnTakeDamage() = 0;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetIsCasting(bool bInIsCasting);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsElectrocuted() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsElectrocuted(bool bInIsElectrocuted);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetThrowableCount();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpendAvailableThrowable();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontage();

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
