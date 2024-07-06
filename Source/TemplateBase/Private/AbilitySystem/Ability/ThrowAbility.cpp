// Retropsis @ 2024

#include "AbilitySystem/Ability/ThrowAbility.h"
#include "Equipment/Weapon/Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacterController.h"

void UThrowAbility::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& CombatSocketTag, bool bOverridePitch, float PitchOverride)
{
	if(!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	// TODO: Data driven Socket
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), CombatSocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if(bOverridePitch)
	{
		Rotation.Pitch = PitchOverride; // Parallel to the ground if needed
	}

	FTransform SpawnTransform;
	SpawnLocation = GetSpawnLocation();
	SpawnDirection = GetSpawnRotation();
	// SpawnTransform.SetLocation(SocketLocation);
	// SpawnTransform.SetRotation(Rotation.Quaternion());
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(SpawnDirection.Quaternion());

	UKismetSystemLibrary::DrawDebugArrow(this, SpawnLocation, SpawnLocation + SpawnDirection.Vector() * 125.f, 2.f, FLinearColor::Blue, 5.f);
		
	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
		ThrowableClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	Projectile->FinishSpawning(SpawnTransform);
}

void UThrowAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,  const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

bool UThrowAbility::CanThrow() const
{
	if(GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_GetThrowableCount(GetAvatarActorFromActorInfo()) > 0;
	}
	return false;
}

void UThrowAbility::UseAvailableThrowable() const
{
	if(GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_SpendAvailableThrowable(GetAvatarActorFromActorInfo());
	}
}

FVector UThrowAbility::GetSpawnLocation() const
{
	return GetAvatarActorFromActorInfo()->GetActorLocation() + GetAvatarActorFromActorInfo()->GetActorForwardVector() * 5.f + 	GetAvatarActorFromActorInfo()->GetActorUpVector() * 25.f;
}

FRotator UThrowAbility::GetSpawnRotation() const
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(GetAvatarActorFromActorInfo()->GetInstigatorController());
	FVector CameraLocation = PlayerCharacterController->PlayerCameraManager->GetCameraLocation();
	FRotator CameraRotation = PlayerCharacterController->PlayerCameraManager->GetCameraRotation();
	
	FVector Start = CameraLocation + UKismetMathLibrary::Conv_RotatorToVector(PlayerCharacterController->PlayerCameraManager->GetCameraRotation()) * StartTraceOffset;
	FVector End = CameraLocation + UKismetMathLibrary::Conv_RotatorToVector(PlayerCharacterController->PlayerCameraManager->GetCameraRotation()) * DefaultTargetingRange;
	UKismetSystemLibrary::LineTraceSingle(this, Start, End, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

	if(HitResult.Distance < 45.f)
	{
		return PlayerCharacterController->GetControlRotation();
	}	
	return HitResult.bBlockingHit ? UKismetMathLibrary::FindLookAtRotation(SpawnLocation, HitResult.Location) : UKismetMathLibrary::FindLookAtRotation(SpawnLocation, End);
}
