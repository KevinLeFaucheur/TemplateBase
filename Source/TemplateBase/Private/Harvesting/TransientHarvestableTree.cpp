// Retropsis @ 2024

#include "Harvesting/TransientHarvestableTree.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ATransientHarvestableTree::ATransientHarvestableTree()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	TrunkCapsule = CreateDefaultSubobject<UCapsuleComponent>("TrunkCapsule");
	TrunkCapsule->SetupAttachment(GetRootComponent());
	TrunkCapsule->SetCollisionObjectType(ECC_PhysicsBody);
	TrunkCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TrunkCapsule->SetCollisionResponseToAllChannels(ECR_Block);
	TrunkCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	TrunkCapsule->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	TrunkCapsule->SetSimulatePhysics(true);

	TreeMesh = CreateDefaultSubobject<UStaticMeshComponent>("TreeMesh");
	TreeMesh->SetupAttachment(TrunkCapsule);
	TreeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATransientHarvestableTree::BeginPlay()
{
	Super::BeginPlay();
	AddForce();
}

void ATransientHarvestableTree::AddForce()
{
	TrunkCapsule->AddForce(Direction * Force, NAME_None, true);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Direction * 115.f, 2.f, FLinearColor::Red, 8.f);

	if(HasAuthority())
	{
		SetLifeSpan(15.f);
	}
}
