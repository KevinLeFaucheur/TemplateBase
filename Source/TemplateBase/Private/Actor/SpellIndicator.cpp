// Retropsis @ 2024

#include "Actor/SpellIndicator.h"
#include "Components/DecalComponent.h"

ASpellIndicator::ASpellIndicator()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpellIndicatorDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	SpellIndicatorDecal->SetupAttachment(GetRootComponent());
}

void ASpellIndicator::SetDecalSize(float Radius) const
{
	SpellIndicatorDecal->DecalSize = FVector(20.f, Radius, Radius);
}

void ASpellIndicator::BeginPlay()
{
	Super::BeginPlay();
}

void ASpellIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

