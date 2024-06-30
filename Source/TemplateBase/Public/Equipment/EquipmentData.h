#pragma once

#include "CoreMinimal.h"
#include "EquipmentData.generated.h"

UENUM(BlueprintType)
enum class EAnimationState : uint8
{
	Default UMETA(DisplayName="Default"),
	Hatchet UMETA(DisplayName="Hatchet"),
	Bow UMETA(DisplayName="Bow"),
	Rock UMETA(DisplayName="Rock"),
	TwoHandedFireWeapon UMETA(DisplayName="TwoHandedFireWeapon"),
	HeavyWeapon UMETA(DisplayName="HeavyWeapon"),
	TwoHandedMeleeWeapon UMETA(DisplayName="TwoHandedMeleeWeapon"),
	Swimming UMETA(DisplayName="Swimming"),
};

USTRUCT(BlueprintType)
struct FEquipmentInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName MainHandSocket = FName();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAnimationState AnimationState = EAnimationState::Default;
	
};

UENUM(BlueprintType)
enum class EToolState : uint8
{
	ETS_Initial UMETA(DisplayName="Initial"),
	ETS_Equipped UMETA(DisplayName="Equipped"),	
	ETS_Dropped UMETA(DisplayName="Dropped"),
	
	ETS_MAX UMETA(DisplayName="DefaultMAX")
};