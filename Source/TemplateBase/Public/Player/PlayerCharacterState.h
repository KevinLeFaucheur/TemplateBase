// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameFramework/PlayerState.h"
#include "PlayerCharacterState.generated.h"

class UPlayerLevelUpInfo;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* NewValue */);

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API APlayerCharacterState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	APlayerCharacterState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FOnPlayerStatChanged OnXPChanged;
	FOnPlayerStatChanged OnLevelChanged;
	FOnPlayerStatChanged OnAttributePointsChanged;
	FOnPlayerStatChanged OnSpellPointsChanged;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPlayerLevelUpInfo> LevelUpInfo;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(ReplicatedUsing=OnRep_Level, VisibleAnywhere)
	int32 Level = 1;
	
	UPROPERTY(ReplicatedUsing=OnRep_XP, VisibleAnywhere)
	int32 XP = 1;
	
	UPROPERTY(ReplicatedUsing=OnRep_AttributePoints, VisibleAnywhere)
	int32 AttributePoints = 0;
	
	UPROPERTY(ReplicatedUsing=OnRep_SpellPoints, VisibleAnywhere)
	int32 SpellPoints = 1;
	
	UPROPERTY(VisibleAnywhere)
	ECharacterClass CharacterClass = ECharacterClass::Novice;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	
	void AddToLevel(int32 InLevel);
	void AddToXP(int32 InXP);
	void AddToAttributePoints(int32 InAttributePoints);
	void AddToSpellPoints(int32 InSpellPoints);
	
	void SetLevel(int32 InLevel);
	void SetXP(int32 InXP);
};
