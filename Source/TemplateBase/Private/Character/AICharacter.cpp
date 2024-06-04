// Retropsis @ 2024

#include "Character/AICharacter.h"
#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AI/BaseAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/BaseUserWidget.h"

AAICharacter::AAICharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AAICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(!HasAuthority()) return;
	
	BaseAIController = Cast<ABaseAIController>(NewController);
	BaseAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BaseAIController->RunBehaviorTree(BehaviorTree);
	BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);

	const bool bIsRanged = CharacterClass == ECharacterClass::Ranger || CharacterClass == ECharacterClass::Mage;
	BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsRanged"), bIsRanged);
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if(HasAuthority())
	{
		UBaseAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

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
		AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &AAICharacter::HitReactTagChanged);
		OnHealthChanged.Broadcast(AS->GetHealth());
		OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
	}
}

void AAICharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	if(HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AAICharacter::InitializeDefaultAttributes() const
{
	UBaseAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAICharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	// TODO: Investigate for client side BaseAIController
	// if(!HasAuthority()) return;
	if(BaseAIController) BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
}

/*
 * Enemy Interface
 */
AActor* AAICharacter::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AAICharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

/*
 * Combat Interface
 */
int32 AAICharacter::GetCharacterLevel()
{
	return Level;
}

void AAICharacter::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

