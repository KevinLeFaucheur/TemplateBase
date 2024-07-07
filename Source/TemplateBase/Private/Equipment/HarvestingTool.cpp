// Retropsis @ 2024

#include "Equipment/HarvestingTool.h"

AHarvestingTool::AHarvestingTool()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ToolClass = EToolClass::HarvestingTool;
}

void AHarvestingTool::DetachToolFromComponent()
{
	const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	Mesh->DetachFromComponent(DetachRules);
}

void AHarvestingTool::OnEquipped()
{
	Super::OnEquipped();
	Mesh->SetSimulatePhysics(false);
	Mesh->SetEnableGravity(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(bUsePhysicsAsset)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetEnableGravity(true);
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
}

void AHarvestingTool::OnDropped()
{
	Super::OnDropped();
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AHarvestingTool::OnSecondary()
{
	Super::OnSecondary();
	Mesh->SetSimulatePhysics(false);
	Mesh->SetEnableGravity(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(bUsePhysicsAsset)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetEnableGravity(true);
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
}
