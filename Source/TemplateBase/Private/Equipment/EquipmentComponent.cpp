// Retropsis @ 2024

#include "Equipment/EquipmentComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Equipment/Tool.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEquipmentComponent, EquippedTool);
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UEquipmentComponent::EquipTool(ATool* ToolToEquip)
{
	if(PlayerCharacter == nullptr || ToolToEquip == nullptr) return;

	EquippedTool = ToolToEquip;
	EquippedTool->SetToolState(EToolState::ETS_Equipped);
	EquippedTool->SetOwner(PlayerCharacter);
	if(const USkeletalMeshSocket* HandSocket = PlayerCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket")))
	{
		HandSocket->AttachActor(EquippedTool, PlayerCharacter->GetMesh());
	}
}

