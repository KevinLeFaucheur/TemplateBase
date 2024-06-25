// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "AbilitySystem/StatusEffect/StatusEffectNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TemplateBase/TemplateBase.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	BurnStatusEffectComponent = CreateDefaultSubobject<UStatusEffectNiagaraComponent>("BurnStatusEffectComponent");
	BurnStatusEffectComponent->SetupAttachment(GetRootComponent());
	BurnStatusEffectComponent->StatusEffectTag = GameplayTags.StatusEffect_Burn;
	
	StunStatusEffectComponent = CreateDefaultSubobject<UStatusEffectNiagaraComponent>("StunStatusEffectComponent");
	StunStatusEffectComponent->SetupAttachment(GetRootComponent());
	StunStatusEffectComponent->StatusEffectTag = GameplayTags.StatusEffect_Stun;

	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachComponent");
	EffectAttachComponent->SetupAttachment(GetRootComponent());
	EffectAttachComponent->SetUsingAbsoluteRotation(true);
	EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
	
	BarrierNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("BarrierNiagaraComponent");
	BarrierNiagaraComponent->SetupAttachment(EffectAttachComponent);
	BarrierNiagaraComponent->PassiveAbilityTag = GameplayTags.Abilities_Passive_Barrier;
	
	HealthDrainNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("HealthDrainNiagaraComponent");
	HealthDrainNiagaraComponent->SetupAttachment(EffectAttachComponent);
	HealthDrainNiagaraComponent->PassiveAbilityTag = GameplayTags.Abilities_Passive_HealthDrain;
	
	ManaDrainNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("ManaDrainNiagaraComponent");
	ManaDrainNiagaraComponent->SetupAttachment(EffectAttachComponent);
	ManaDrainNiagaraComponent->PassiveAbilityTag = GameplayTags.Abilities_Passive_ManaDrain;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, bIsStunned);
	DOREPLIFETIME(ABaseCharacter, bIsBurning);
	DOREPLIFETIME(ABaseCharacter, bIsElectrocuted);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ABaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ABaseCharacter::AddCharacterAbilities()
{
	UBaseAbilitySystemComponent* ASC = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	if(!HasAuthority()) return;

	ASC->AddCharacterAbilities(StartupAbilities);
	ASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void ABaseCharacter::PlayHitReactMontage()
{
	IPlayerInterface::PlayHitReactMontage();
}

float ABaseCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	OnTakeDamage.Broadcast(DamageTaken);
	return DamageTaken;
}

/*
 * Combat Interface
 */
FVector ABaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	// TODO: Implements this for both weapons or just hands
	const FBaseGameplayTags&GameplayTags = FBaseGameplayTags::Get();
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
}

USkeletalMeshComponent* ABaseCharacter::GetWeapon_Implementation()
{
	return Weapon;
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ABaseCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

void ABaseCharacter::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse);
}

void ABaseCharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(DeathSound) UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	Dissolve();
	bDead = true;
	BurnStatusEffectComponent->Deactivate();
	StunStatusEffectComponent->Deactivate();
	OnDeath.Broadcast(this);
}

bool ABaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

AActor* ABaseCharacter::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> ABaseCharacter::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

FTaggedMontage ABaseCharacter::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if(TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

UNiagaraSystem* ABaseCharacter::GetImpactEffect_Implementation()
{
	return ImpactEffect;
}

int32 ABaseCharacter::GetMinionCount_Implementation()
{
	return MinionCount;
}

void ABaseCharacter::IncrementMinionCount_Implementation(const int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass ABaseCharacter::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnTakeDamage& ABaseCharacter::GetOnTakeDamage()
{
	return OnTakeDamage;
}

FOnASCRegistered& ABaseCharacter::GetOnASCRegistered()
{
	return OnASCRegistered;
}

FOnDeath& ABaseCharacter::GetOnDeathDelegate()
{
	return OnDeath;
}

bool ABaseCharacter::IsElectrocuted_Implementation() const
{
	return bIsElectrocuted;
}

void ABaseCharacter::SetIsElectrocuted_Implementation(bool bInIsElectrocuted)
{
	bIsElectrocuted = bInIsElectrocuted;
}

/*
 * Visual Effects
 */
void ABaseCharacter::Dissolve()
{
	TArray<UMaterialInstanceDynamic*> DynamicInstances;
	for (int i = 0; i < DissolveMaterialInstances.Num(); ++i)
	{
		if (IsValid(DissolveMaterialInstances[i]))
		{
			UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstances[i], this);
			GetMesh()->SetMaterial(i, DynamicMatInst);
			DynamicInstances.Add(DynamicMatInst);
		}
	}
	if(DynamicInstances.Num() > 0) StartDissolveTimeline(DynamicInstances);
	
	if(IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

/*
 *
 */
UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
