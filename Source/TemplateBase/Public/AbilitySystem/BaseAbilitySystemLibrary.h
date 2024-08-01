// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilityTypes.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaseAbilitySystemLibrary.generated.h"

class ATool;
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
	/*
	 * Widget Controller
	 */
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static  bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, APlayerHUD*& OutPlayerHUD);

	/*
	 * Initializing Ability System
	 */
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void GiveToolAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ATool* Tool);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void RemoveToolAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ATool* Tool);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void GiveMonkAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void RemoveMonkAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static UAbilityInfo* GetToolAbilityInfo(const UObject* WorldContextObject);
	
	/*
	 * Effect Context Getters  
	 */
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
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static float GetShowDamageDelay(const FGameplayEffectContextHandle& EffectContextHandle);
		
	/*
	 * Effect Context Setters  
	 */
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

	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetAirborneForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InAirborneForce);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InIsRadialDamage);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRadialOrigin);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void SetShowDamageDelay(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDelay);

	/*
	 * Gameplay Mechanics
	 */
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary")
	static bool IsHostile(const AActor* FirstActor, const AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary")
	static TArray<FVector> EvenlySpacedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);
	
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

	/*
	 * Damage Effect Params
	*/
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary")
	static float GetRadialDamageWithFalloff(const AActor* TargetActor, float BaseDamage, float MinimumDamage, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary")
	static void SetIsRadialDamageEffectParam(UPARAM(ref) FDamageEffectParams& DamageEffectParams, bool IsRadialDamage, float InnerRadius, float OuterRadius, FVector Origin);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary")
	static void SetAirborneDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector AirborneDirection, float Magnitude = 0.f);
	
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary")
	static void SetDeathImpulseDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector DeathImpulseDirection, float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary")
	static void SetTargetAbilitySystemComponent(UPARAM(ref) FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InTargetASC);

	/*
	 * Healing Effect Params
	*/
	UFUNCTION(BlueprintCallable, Category="BaseAbilitySystemLibrary")
	static float GetRadialHealingWithFalloff(const AActor* TargetActor, float BaseHealing, float MinimumHealing, const FVector& Origin, float HealingInnerRadius, float HealingOuterRadius, float HealingFalloff);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static bool IsRadialHealing(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static float GetRadialHealingInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static float GetRadialHealingOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="BaseAbilitySystemLibrary", meta=(WorldContext="WorldContextObject"))
	static FVector GetRadialHealingOrigin(const FGameplayEffectContextHandle& EffectContextHandle);
};


