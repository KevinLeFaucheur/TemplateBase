// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilityTypes.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaseAbilitySystemLibrary.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
struct FWidgetControllerParams;
struct FGameplayEffectContextHandle;
class APlayerHUD;
class USpellMenuWidgetController;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UBaseAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, APlayerHUD*& OutPlayerHUD);

	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static bool IsSuccessfulStatusEffect(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static float GetStatusEffectDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static float GetStatusEffectDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static float GetStatusEffectFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static FGameplayTag GetStatusEffectDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static FVector GetAirborneForce(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool IsInBlockedHit);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool IsInCriticalHit);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetIsSuccessfulStatusEffect(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsSuccessful);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetStatusEffectDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDamage);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetStatusEffectDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDuration);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetStatusEffectFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InFrequency);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetAirborneForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InAirborneForce);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary"/*, meta=(WorldContext="WorldContextObject")*/)
	static bool IsHostile(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary")
	static TArray<FVector> EvenlySpacedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);
	
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
	
};


