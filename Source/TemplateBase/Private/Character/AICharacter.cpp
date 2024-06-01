// Retropsis @ 2024


#include "Character/AICharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/BaseUserWidget.h"

AAICharacter::AAICharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

	if (UBaseUserWidget* BaseUserWidget = Cast<UBaseUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		BaseUserWidget->SetWidgetController(this);
	}
	
	if(const UBaseAttributeSet* AS = CastChecked<UBaseAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
        	[this] (const FOnAttributeChangeData& Data)
        	{
        		OnHealthChanged.Broadcast(Data.NewValue);
        	}
        );
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
        	[this] (const FOnAttributeChangeData& Data)
        	{
        		OnMaxHealthChanged.Broadcast(Data.NewValue);
        	}
        );
		OnHealthChanged.Broadcast(AS->GetHealth());
		OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
	}
}

void AAICharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	InitializeDefaultAttributes();
}

void AAICharacter::InitializeDefaultAttributes() const
{
	UBaseAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

int32 AAICharacter::GetCharacterLevel()
{
	return Level;
}
