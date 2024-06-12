// Retropsis @ 2024

#include "AbilitySystem/BaseAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "AbilitySystem/AbilityTypes.h"
#include "Engine/OverlapResult.h"
#include "Game/OverworldGameMode.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacterState.h"
#include "UI/PlayerHUD.h"
#include "UI/Controller/BaseWidgetController.h"

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

int32 UBaseAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
                                                             ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

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

void UBaseAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
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

bool UBaseAbilitySystemLibrary::IsHostile(AActor* FirstActor, AActor* SecondActor)
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
