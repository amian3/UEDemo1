// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAICharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include"AIController.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "SamuraiCharacter.h"
// Sets default values
AArcherAICharacter::AArcherAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	check(Weapon != nullptr)
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LeftHandWeapon"));

	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	HPBar->SetupAttachment(RootComponent);

	Cone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cone"));
	Cone->SetupAttachment(RootComponent);
	Cone->SetVisibility(false);

	Health = 100;
	MaxHealth = 100;
	MoveStatus = EArcherMoveStatus::MS_Idle;
}

// Called when the game starts or when spawned
void AArcherAICharacter::BeginPlay()
{
	Super::BeginPlay();

	Instance = GetMesh()->GetAnimInstance();
	AIController = Cast<AAIController>(GetController());
}

// Called every frame
void AArcherAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AArcherAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AArcherAICharacter::Die()
{
	PlayAnimMontage(Death);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MoveStatus = EArcherMoveStatus::MS_Dead;
	//Destroy();
}