// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/PlayerInterface.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "BaseCharacter.generated.h"

class UPassiveNiagaraComponent;
class UStatusEffectNiagaraComponent;
class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;


UCLASS(Abstract)
class TEMPLATEBASE_API ABaseCharacter : public ACharacter, public IPlayerInterface, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//~ Player Interface
	virtual void PlayHitReactMontage() override;
	//~ Player Interface

	//~ Combat Interface
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual UNiagaraSystem* GetImpactEffect_Implementation() override;
	virtual 	int32 GetMinionCount_Implementation() override;
	virtual void IncrementMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnTakeDamage& GetOnTakeDamage() override;
	virtual FOnASCRegistered& GetOnASCRegistered() override;
	virtual FOnDeath& GetOnDeathDelegate() override;
	virtual bool IsElectrocuted_Implementation() const override;
	virtual void SetIsElectrocuted_Implementation(bool bInIsElectrocuted) override;
	//~ Combat Interface

	FOnTakeDamage OnTakeDamage;
	FOnASCRegistered OnASCRegistered;
	FOnDeath OnDeath;
	
	UPROPERTY(EditAnywhere, Category="Character|Montages")
	TArray<FTaggedMontage> AttackMontages;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(ReplicatedUsing=OnRep_IsStunned, BlueprintReadOnly)
	bool bIsStunned = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_IsBurning, BlueprintReadOnly)
	bool bIsBurning = false;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsElectrocuted = false;

	UFUNCTION()
	virtual void OnRep_IsStunned() {}
	
	UFUNCTION()
	virtual void OnRep_IsBurning() {}
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() {}

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterAbilities();

	UFUNCTION()
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS|Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS|Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category="Character")
	FName WeaponSocketName;
	
	UPROPERTY(EditAnywhere, Category="Character")
	FName LeftHandSocketName;
	
	UPROPERTY(EditAnywhere, Category="Character")
	FName RightHandSocketName;
	
	UPROPERTY(EditAnywhere, Category="Character")
	FName TailSocketName;

	bool bDead = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStatusEffectNiagaraComponent> BurnStatusEffectComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStatusEffectNiagaraComponent> StunStatusEffectComponent;

	/*
	 * Minions
	 */
	int32 MinionCount = 0;

	/*
	 * Dissolve Material
	 */
	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(const TArray<UMaterialInstanceDynamic*>& DynamicMaterialInstances);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Effects")
	TArray<TObjectPtr<UMaterialInstance>> DissolveMaterialInstances;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Effects")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Effects")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Effects")
	TObjectPtr<USoundBase> DeathSound;

private:	
	UPROPERTY(EditAnywhere, Category="GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditAnywhere, Category="GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	// TODO: Putting this back to CharacterAnimInstance, though enemies need it too
	UPROPERTY(EditAnywhere, Category="Character|Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> BarrierNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HealthDrainNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaDrainNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
};
