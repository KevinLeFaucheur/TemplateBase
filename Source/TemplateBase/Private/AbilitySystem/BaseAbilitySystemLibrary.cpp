// Retropsis @ 2024

#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "AbilitySystem/AbilityTypes.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "Equipment/Tool.h"
#include "Game/OverworldGameMode.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacterState.h"
#include "UI/PlayerHUD.h"
#include "UI/Controller/BaseWidgetController.h"

/*
 * Widget Controller
 */
UOverlayWidgetController* UBaseAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	APlayerHUD* PlayerHUD = nullptr;
	if(MakeWidgetControllerParams(WorldContextObject, WCParams, PlayerHUD))
	{
		return PlayerHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UBaseAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	APlayerHUD* PlayerHUD = nullptr;
	if(MakeWidgetControllerParams(WorldContextObject, WCParams, PlayerHUD))
	{
		return PlayerHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UBaseAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	APlayerHUD* PlayerHUD = nullptr;
	if(MakeWidgetControllerParams(WorldContextObject, WCParams, PlayerHUD))
	{
		return PlayerHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

bool UBaseAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, APlayerHUD*& OutPlayerHUD)
{
	if (const APlayerController* PC  = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		OutPlayerHUD = Cast<APlayerHUD>(PC->GetHUD());
		if(OutPlayerHUD)
		{
			const APlayerCharacterState* PS = PC->GetPlayerState<APlayerCharacterState>();
			
			OutWCParams.PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController();
			OutWCParams.PlayerState = PC->GetPlayerState<APlayerCharacterState>();
			OutWCParams.AbilitySystemComponent = PS->GetAbilitySystemComponent();
			OutWCParams.AttributeSet = PS->GetAttributeSet();
			return true;
		}
	}
	return false;
}

/*
 * Initializing Ability System
 */
void UBaseAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	const AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesEffectContext = ASC->MakeEffectContext();
	PrimaryAttributesEffectContext.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle SecondaryAttributesEffectContext = ASC->MakeEffectContext();
	SecondaryAttributesEffectContext.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle VitalAttributesEffectContext = ASC->MakeEffectContext();
	VitalAttributesEffectContext.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UBaseAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{	
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;
	
	for (const TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	
	const FCharacterClassDefaultInfo DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if(ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetCharacterLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

void UBaseAbilitySystemLibrary::GiveToolAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ATool* Tool)
{
	UAbilityInfo* ToolAbilityInfo = GetToolAbilityInfo(WorldContextObject);
	if(ToolAbilityInfo == nullptr || Tool == nullptr) return;
	
	for (TTuple<FGameplayTag, FGameplayTag> Ability : Tool->ToolAbilities)
	{
		const FBaseAbilityInfo AbilityInfo = ToolAbilityInfo->FindAbilityInfoForTag(Ability.Key);
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityInfo.Ability, 1);
		AbilitySpec.DynamicAbilityTags.AddTag(Ability.Value);
		ASC->GiveAbility(AbilitySpec);
		ASC->MarkAbilitySpecDirty(AbilitySpec);
		
		if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(ASC))
		{
			BaseASC->ClientEquipToolAbility(Ability.Key, FBaseGameplayTags::Get().Abilities_Status_Equipped, Ability.Value, FGameplayTag());
		}
	}

	// TODO: Could give extra skills if character has a specific class or weapon mastery skill 
	// const FCharacterClassDefaultInfo DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	// for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	// {
	// 	if(ASC->GetAvatarActor()->Implements<UCombatInterface>())
	// 	{
	// 		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetCharacterLevel(ASC->GetAvatarActor()));
	// 		ASC->GiveAbility(AbilitySpec);
	// 	}
	// }
}

void UBaseAbilitySystemLibrary::RemoveToolAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ATool* Tool)
{
	UAbilityInfo* ToolAbilityInfo = GetToolAbilityInfo(WorldContextObject);
	if(ToolAbilityInfo == nullptr || Tool == nullptr) return;
	
	for (TTuple<FGameplayTag, FGameplayTag> Ability : Tool->ToolAbilities)
	{
		if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(ASC))
		{
			if(FGameplayAbilitySpec* AbilitySpec = BaseASC->GetSpecFromAbilityTag(Ability.Key))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(Ability.Value);
				ASC->ClearAbility(AbilitySpec->Handle);
				ASC->MarkAbilitySpecDirty(*AbilitySpec);
				
				const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
				BaseASC->ClientEquipToolAbility(GameplayTags.Abilities_None, GameplayTags.Abilities_Status_Unlocked, Ability.Value, FGameplayTag());
			}
		}
	}
}

void UBaseAbilitySystemLibrary::GiveMonkAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(ASC))
	{
		const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
		if(FGameplayAbilitySpec* AbilitySpec = BaseASC->GetSpecFromAbilityTag(GameplayTags.Abilities_Monk_Basic))
		{
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.InputTag_LMB);
			ASC->GiveAbility(*AbilitySpec);
			ASC->MarkAbilitySpecDirty(*AbilitySpec);
					
			BaseASC->ClientEquipToolAbility(GameplayTags.Abilities_Monk_Basic, GameplayTags.Abilities_Status_Equipped, GameplayTags.InputTag_LMB, FGameplayTag());
		}
	}
}

void UBaseAbilitySystemLibrary::RemoveMonkAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(ASC))
	{
		const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
		if(FGameplayAbilitySpec* AbilitySpec = BaseASC->GetSpecFromAbilityTag(GameplayTags.Abilities_Monk_Basic))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.InputTag_LMB);
			ASC->ClearAbility(AbilitySpec->Handle);
			ASC->MarkAbilitySpecDirty(*AbilitySpec);
					
			BaseASC->ClientEquipToolAbility(GameplayTags.Abilities_None, GameplayTags.Abilities_Status_Unlocked, GameplayTags.InputTag_LMB, FGameplayTag());
		}
	}
}

UCharacterClassInfo* UBaseAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AOverworldGameMode* OverworldGameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(OverworldGameMode == nullptr) return nullptr;
	return  OverworldGameMode->CharacterClassInfo;
}

UAbilityInfo* UBaseAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AOverworldGameMode* OverworldGameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(OverworldGameMode == nullptr) return nullptr;
	return  OverworldGameMode->AbilityInfo;
}

UAbilityInfo* UBaseAbilitySystemLibrary::GetToolAbilityInfo(const UObject* WorldContextObject)
{
	const AOverworldGameMode* OverworldGameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(OverworldGameMode == nullptr) return nullptr;
	return  OverworldGameMode->ToolAbilityInfo;
}

int32 UBaseAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

/*
 * Effect Context Getters  
 */
bool UBaseAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->IsBlockedHit();
	}
	return false;
}

bool UBaseAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->IsCriticalHit();
	}
	return false;
}

bool UBaseAbilitySystemLibrary::IsSuccessfulStatusEffect(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->IsSuccessfulStatusEffect();
	}
	return false;
}

float UBaseAbilitySystemLibrary::GetStatusEffectDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetStatusEffectDamage();
	}
	return 0.f;
}

float UBaseAbilitySystemLibrary::GetStatusEffectDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetStatusEffectDuration();
	}
	return 0.f;
}

float UBaseAbilitySystemLibrary::GetStatusEffectFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetStatusEffectFrequency();
	}
	return 0.f;
}

FGameplayTag UBaseAbilitySystemLibrary::GetStatusEffectDamageType(
	const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if(BaseEffectContext->GetDamageType().IsValid())
		{
			return *BaseEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UBaseAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if(BaseEffectContext->GetDamageType().IsValid())
		{
			return BaseEffectContext->GetDeathImpulse();
		}
	}
	return FVector::ZeroVector;
}

FVector UBaseAbilitySystemLibrary::GetAirborneForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if(BaseEffectContext->GetDamageType().IsValid())
		{
			return BaseEffectContext->GetAirborneForce();
		}
	}
	return FVector::ZeroVector;
}

bool UBaseAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->IsRadialDamage();
	}
	return false;
}

float UBaseAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UBaseAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UBaseAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

float UBaseAbilitySystemLibrary::GetShowDamageDelay(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetShowDamageDelay();
	}
	return 0.f;
}

/*
 * Effect Context Setters  
 */
void UBaseAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool IsInBlockedHit)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetIsBlockedHit(IsInBlockedHit);
	}
}

void UBaseAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool IsInCriticalHit)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetIsCriticalHit(IsInCriticalHit);
	}
}

void UBaseAbilitySystemLibrary::SetIsSuccessfulStatusEffect(FGameplayEffectContextHandle& EffectContextHandle, bool bIsSuccessful)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetIsSuccessfulStatusEffect(bIsSuccessful);
	}
}

void UBaseAbilitySystemLibrary::SetStatusEffectDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetStatusEffectDamage(InDamage);
	}
}

void UBaseAbilitySystemLibrary::SetStatusEffectDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetStatusEffectDuration(InDuration);
	}
}

void UBaseAbilitySystemLibrary::SetStatusEffectFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetStatusEffectFrequency(InFrequency);
	}
}

void UBaseAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetDamageType(MakeShared<FGameplayTag>(InDamageType));
	}
}

void UBaseAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InDeathImpulse)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UBaseAbilitySystemLibrary::SetAirborneForce(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InAirborneForce)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetAirborneForce(InAirborneForce);
	}
}

void UBaseAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,
	bool InIsRadialDamage)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetIsRadialDamage(InIsRadialDamage);
	}
}

void UBaseAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UBaseAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UBaseAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRadialOrigin)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetRadialDamageOrigin(InRadialOrigin);
	}
}

void UBaseAbilitySystemLibrary::SetShowDamageDelay(FGameplayEffectContextHandle& EffectContextHandle, float InDelay)
{
	if (FBaseGameplayEffectContext* BaseEffectContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseEffectContext->SetShowDamageDelay(InDelay);
	}
}

/*
 * Gameplay Mechanics
 */
void UBaseAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	// query scene to see what we hit
	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(
			Overlaps, SphereOrigin, FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if(Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void UBaseAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if(Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;;
	}
	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;

	while (NumTargetsFound < MaxTargets)
	{
		if(ActorsToCheck.Num() == 0) break;
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;
		for (AActor* PotentialTarget : ActorsToCheck)
		{
			const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
			if(Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = PotentialTarget;
			}
		}
		ActorsToCheck.Remove(ClosestActor);
		OutClosestTargets.AddUnique(ClosestActor);
		++NumTargetsFound;
	}
}

bool UBaseAbilitySystemLibrary::IsHostile(const AActor* FirstActor, const AActor* SecondActor)
{
	return ! (FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"))
		|| FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy")));
}

FGameplayEffectContextHandle UBaseAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetAirborneForce(EffectContextHandle, DamageEffectParams.AirborneForce);

	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);
	SetShowDamageDelay(EffectContextHandle, DamageEffectParams.ShowDamageDelay);
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
	
	const float ScaledMagnitudeMin = DamageEffectParams.BaseDamageRange.DamageMin.Value;
	const float ScaledMagnitudeMax = DamageEffectParams.BaseDamageRange.DamageMax.Value;
	const float Magnitude = FMath::FRandRange(ScaledMagnitudeMin, ScaledMagnitudeMax);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, Magnitude);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Chance, DamageEffectParams.StatusEffectChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Damage, DamageEffectParams.StatusEffectDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Duration, DamageEffectParams.StatusEffectDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.StatusEffect_Frequency, DamageEffectParams.StatusEffectFrequency);
		
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	return EffectContextHandle;
}

TArray<FRotator> UBaseAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	
	if(NumRotators > 1)
	{
		const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Rotators.Add(Direction.Rotation());
		}
	}
	else Rotators.Add(Forward.Rotation());
	return Rotators;
}

TArray<FVector> UBaseAbilitySystemLibrary::EvenlySpacedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
	
	if(NumVectors > 1)
	{
		const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Vectors.Add(Direction);
		}
	}
	else Vectors.Add(Forward);
	return Vectors;
}

/*
 * Damage Effect Params
 */
void UBaseAbilitySystemLibrary::SetIsRadialDamageEffectParam(FDamageEffectParams& DamageEffectParams, bool IsRadialDamage, float InnerRadius, float OuterRadius, FVector Origin)
{
	DamageEffectParams.bIsRadialDamage = IsRadialDamage;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
}

void UBaseAbilitySystemLibrary::SetAirborneDirection(FDamageEffectParams& DamageEffectParams, FVector AirborneDirection, float Magnitude)
{
	AirborneDirection.Normalize();
	if(Magnitude == 0.f) DamageEffectParams.AirborneForce = AirborneDirection * DamageEffectParams.AirborneForceMagnitude;
	else DamageEffectParams.AirborneForce = AirborneDirection * Magnitude;
}

void UBaseAbilitySystemLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams, FVector DeathImpulseDirection, float Magnitude)
{
	DeathImpulseDirection.Normalize();
	if(Magnitude == 0.f) DamageEffectParams.DeathImpulse = DeathImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
	else DamageEffectParams.DeathImpulse = DeathImpulseDirection * Magnitude;
}

void UBaseAbilitySystemLibrary::SetTargetAbilitySystemComponent(FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InTargetASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InTargetASC;
}

float UBaseAbilitySystemLibrary::GetRadialDamageWithFalloff(const AActor* TargetActor, float BaseDamage,  float MinimumDamage, const FVector& Origin, float DamageInnerRadius, float DamageOuterRadius, float DamageFalloff)
{
	if (!TargetActor) return 0.f;
 
	FRadialDamageParams RadialDamageParams;
	RadialDamageParams.BaseDamage = BaseDamage;
	RadialDamageParams.DamageFalloff = DamageFalloff;
	RadialDamageParams.InnerRadius = DamageInnerRadius;
	RadialDamageParams.OuterRadius = DamageOuterRadius;
	RadialDamageParams.MinimumDamage = MinimumDamage;
	const float DamageScale = RadialDamageParams.GetDamageScale((Origin - TargetActor->GetActorLocation()).Length());
	
	return BaseDamage * DamageScale;
}

// TODO: To Healing variables
float UBaseAbilitySystemLibrary::GetRadialHealingWithFalloff(const AActor* TargetActor, float BaseHealing, float MinimumHealing, const FVector& Origin, float HealingInnerRadius, float HealingOuterRadius, float HealingFalloff)
{
	if (!TargetActor) return 0.f;
 
	FRadialDamageParams RadialDamageParams;
	RadialDamageParams.BaseDamage = BaseHealing;
	RadialDamageParams.DamageFalloff = HealingFalloff;
	RadialDamageParams.InnerRadius = HealingInnerRadius;
	RadialDamageParams.OuterRadius = HealingOuterRadius;
	RadialDamageParams.MinimumDamage = MinimumHealing;
	const float DamageScale = RadialDamageParams.GetDamageScale((Origin - TargetActor->GetActorLocation()).Length());
	
	return BaseHealing * DamageScale;
}

bool UBaseAbilitySystemLibrary::IsRadialHealing(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->IsRadialDamage();
	}
	return false;
}

float UBaseAbilitySystemLibrary::GetRadialHealingInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UBaseAbilitySystemLibrary::GetRadialHealingOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UBaseAbilitySystemLibrary::GetRadialHealingOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseEffectContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}
