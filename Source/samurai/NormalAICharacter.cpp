// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalAICharacter.h"
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
ANormalAICharacter::ANormalAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	check(Weapon != nullptr)
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("katana3"));

	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CombatBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatBox"));
	CombatBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("katana3"));

	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	HPBar->SetupAttachment(RootComponent);

	Cone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cone"));
	Cone->SetupAttachment(RootComponent);
	Cone->SetVisibility(false);

	Damage = 5;
	Health = 100;
	MaxHealth = 100;
	MoveStatus = ENormalMoveStatus::MS_Idle;
}

// Called when the game starts or when spawned
void ANormalAICharacter::BeginPlay()
{
	Super::BeginPlay();

	Instance = GetMesh()->GetAnimInstance();
	AIController = Cast<AAIController>(GetController());

	CombatBox->OnComponentBeginOverlap.AddDynamic(this, &ANormalAICharacter::OnCombatBoxOverlapBegin);
	CombatBox->OnComponentEndOverlap.AddDynamic(this, &ANormalAICharacter::OnCombatBoxOverlapEnd);
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ANormalAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANormalAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANormalAICharacter::OnCombatBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ASamuraiCharacter* Man = Cast<ASamuraiCharacter>(OtherActor);
		if (Man)
		{
			if (Man->InteractParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, Man->InteractParticle, GetMesh()->GetSocketLocation("katana3"));
			}//UGameplayStatics::SpawnEmitterAtLocation(this, Enemy->InteractParticle, GetMesh()->GetSocketLocation("katana3"));
			if (Man->ReactSound)
			{
				UGameplayStatics::PlaySound2D(this, Man->ReactSound);
			}
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Man, Damage, AIController, this, DamageTypeClass);
			}
		}
	}
}

void ANormalAICharacter::OnCombatBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ANormalAICharacter::EnableCollision()
{
	CombatBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ANormalAICharacter::DisableCollision()
{
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


/*
float ANormalAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (Health > DamageAmount)
	{
		Health -= DamageAmount;
	}
	else
	{
		Health = 0;
		Die();
	}
	return DamageAmount;
}*/

void ANormalAICharacter::Die()
{
	PlayAnimMontage(Death);
	DisableCollision();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MoveStatus = ENormalMoveStatus::MS_Dead;
	//Destroy();
}