// Retropsis @ 2024

#include "Equipment/Weapon/Casing.h"

#include "Kismet/GameplayStatics.h"

ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetEnableGravity(true);
	SetRootComponent(Mesh);
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);
	Mesh->AddImpulse(GetActorForwardVector() * CasingEjectionImpulse);
}

void ACasing::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	SetLifeSpan(5.f);
	if(CasingSound)
	{
		Mesh->SetNotifyRigidBodyCollision(false);
		UGameplayStatics::PlaySoundAtLocation(this, CasingSound, GetActorLocation());
	}
}
