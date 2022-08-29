// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
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
AAICharacter::AAICharacter()
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
	MoveStatus = EMoveStatus::MS_Idle;
	//HitAccount = 0;
//	MaxHitAccount = 10;

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	Instance = GetMesh()->GetAnimInstance();
	AIController = Cast<AAIController>(GetController());

	CombatBox->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnCombatBoxOverlapBegin);
	CombatBox->OnComponentEndOverlap.AddDynamic(this, &AAICharacter::OnCombatBoxOverlapEnd);
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}




void AAICharacter::OnCombatBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ASamuraiCharacter* Man = Cast<ASamuraiCharacter>(OtherActor);
		if (Man)
		{
			if (UGameplayStatics::ApplyDamage(Man, Damage, AIController, this, DamageTypeClass) == 0)
			{
				if (Man->SteelParticle)
				{
					UGameplayStatics::SpawnEmitterAtLocation(this, Man->SteelParticle, GetMesh()->GetSocketLocation("katana3"));
				}//UGameplayStatics::SpawnEmitterAtLocation(this, Enemy->InteractParticle, GetMesh()->GetSocketLocation("katana3"));
				if (Man->SteelSound)
				{
					UGameplayStatics::PlaySound2D(this, Man->SteelSound);
				}
			}
			else
			{
				if (Man->InteractParticle)
				{
					UGameplayStatics::SpawnEmitterAtLocation(this, Man->InteractParticle, GetMesh()->GetSocketLocation("katana3"));
				}//UGameplayStatics::SpawnEmitterAtLocation(this, Enemy->InteractParticle, GetMesh()->GetSocketLocation("katana3"));
				if (Man->ReactSound)
				{
					UGameplayStatics::PlaySound2D(this, Man->ReactSound);
				}

			}
		}
	}
}

void AAICharacter::OnCombatBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AAICharacter::EnableCollision()
{
	CombatBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAICharacter::DisableCollision()
{
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


/*
float AAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (Health > DamageAmount)
	{
		Health -= DamageAmount;
		HitAccount += 1;
		if (HitAccount == 10)
		{
			PlayAnimMontage(Hitted);
			HitAccount = 0;
		}

	}
	else
	{
		Health = 0;
		Die();
	}
	return DamageAmount;
}*/

void AAICharacter::Die()
{
	PlayAnimMontage(Death);
	DisableCollision();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MoveStatus = EMoveStatus::MS_Dead;
	//Destroy();
}
/*
void AAICharacter::DeathEnd()
{
	Instance->Montage_Pause();
	Destroy();
}*/