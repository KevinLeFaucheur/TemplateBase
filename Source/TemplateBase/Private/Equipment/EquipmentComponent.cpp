// Retropsis @ 2024

#include "Equipment/EquipmentComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Equipment/Tool.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	BaseWalkSpeed = 600.f;;
	AimWalkSpeed = 400.f;;
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEquipmentComponent, EquippedTool);
	DOREPLIFETIME(UEquipmentComponent, bAiming);
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	if(PlayerCharacter) PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/*
 * Equipping
 */
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
	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	PlayerCharacter->bUseControllerRotationYaw = true;
}

void UEquipmentComponent::OnRep_EquippedTool()
{
	if(EquippedTool && PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		PlayerCharacter->bUseControllerRotationYaw = true;
	}	
}

/*
 * Aiming
 */
void UEquipmentComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	if(PlayerCharacter) PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
}

void UEquipmentComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if(PlayerCharacter) PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
}
