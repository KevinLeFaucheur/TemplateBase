// Retropsis @ 2024

#include "Equipment/MeleeWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AMeleeWeapon::AMeleeWeapon()
{
	ToolClass = EToolClass::MeleeWeapon;
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	// CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
	
	MarksmanFOV = 90.f;
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
		CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnBoxBeginOverlap);
	}
}

void AMeleeWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	// UKismetSystemLibrary::DrawDebugBox(this, Start, FVector(5.f), FLinearColor::Yellow, FRotator::ZeroRotator, 5.f);
	// UKismetSystemLibrary::DrawDebugBox(this, End, FVector(5.f), FLinearColor::Yellow, FRotator::ZeroRotator, 5.f);

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for(AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::DrawDebugSphere(this, SweepResult.Location, 25.f, 12, FLinearColor::Green, 5.f);
	
	UKismetSystemLibrary::BoxTraceSingle(
		this, Start, End,
		FVector(5.f, 5.f, 5.f),
		BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::White,
		5.f
	);
	AActor* HitActor = HitResult.GetActor();
	
	// if(!bIsMultiHit && HitActor)
	// {
	// 	IgnoreActors.AddUnique(HitActor);
	// }
	if(HitResult.GetComponent())
	{
		GEngine->AddOnScreenDebugMessage(654, 5.f, FColor::Green, FString::Printf(TEXT("Has Hit %s"), *HitResult.GetComponent()->GetName()));
	}
	
	CauseDamage(HitResult);
	if(HitActor)
	{
		if(TaggedMontage.ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, TaggedMontage.ImpactSound, GetActorLocation());
		}
		if(HitActor->Implements<UCombatInterface>())
		{
			UNiagaraSystem* ImpactEffect = ICombatInterface::Execute_GetImpactEffect(HitActor);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		}
	}
}

void AMeleeWeapon::EmptyIgnoreActors()
{
	IgnoreActors.Empty();
}
