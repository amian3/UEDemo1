

#include "StandardAI.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
AStandardAI::AStandardAI()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStandardAI::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AStandardAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AStandardAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

