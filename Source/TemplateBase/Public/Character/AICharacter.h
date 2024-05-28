// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AAICharacter();
	
	//~ Combat Interface
	virtual int32 GetCharacterLevel();
	//~ Combat Interface

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	int32 Level = 1;

private:
};
