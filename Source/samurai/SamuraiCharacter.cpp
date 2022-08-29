// Fill out your copyright notice in the Description page of Project Settings.


#include "SamuraiCharacter.h"

//#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "AICharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "SamuraiController.h"
#include "StandardAI.h"
#include "NormalAICharacter.h"
#include "Projectile.h"

// Sets default values


ASamuraiCharacter::ASamuraiCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	check(SpringArmComp != nullptr)
		SpringArmComp->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	//FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	check(CameraComp != nullptr)
		CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);

	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->TargetArmLength = 400.0f;
	//SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 600.0f));
	//SpringArmComp->setcoll

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	SpringArmComp->bUsePawnControlRotation = true;
	CameraComp->bUsePawnControlRotation = false;

	katana3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("katana3"));
	check(katana3 != nullptr)
		katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("scabbard1"));

	scabbard1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("scabbard1"));

	check(scabbard1 != nullptr)
		scabbard1->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("scabbard1"));

	Bow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("weapon3"));
	check(Bow != nullptr)
		Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bow"));


	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	katana3->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	scabbard1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Bow->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	CombatBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatBox"));
	//CombatBox->SetupAttachment(Mesh);
	CombatBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("katana3"));



	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphere"));
	DetectSphere->SetupAttachment(RootComponent);


	CurrentAttack = 0;
	SaveAttack = -1;
	IsInAir = false;
	Health = 100.0f;
	MaxHealth = 100.0f;

	IsFocusOn = false;
	IsEvading = false;
	IsDefensed = false;


	KatanaDamage = 30.0f;
	KatanaHeavyDamage = 60.0f;

	DamageRatio = 1.0f;

	Tenacity = 100;
	MaxTenacity = 100;
	TenacityRatio = 1.0f;
	TenacityConsumeRate = 0.1f;
	TenacityRecoverRate = 5.0f;


	CurrentStatus = EStatus::ES_Normal;
	RunSpeed = 600.0f;
	EquipSpeed = 400.0f;
	SpeedRatio = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed * SpeedRatio;

	IsAttacking = false;
	IsAttacking1 = false;
	IsAttacking2 = false;

	NumOfRecover = 5;
	NumOfArrow = 10;
	RecoverHP = 30;
	IsPrepared = false;
	IsPreparedInHeavy = false;

	ConsumeOfArrow = 2;
	HeavyConsumeOfArrow = 5;
	SufferAccount = 0;
	MaxSufferAccount = 1;

	AxisX = 0.0f;
	AxisY = 0.0f;
	AxisZ = 0.0f;
	CurrentAxisX = 0.0f;
	CurrentAxisY = 0.0f;
	IsDead = false;




	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(GetMesh());
	//FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	GunOffset = FVector(100.0f, 0.0f, 10.0f);
}

// Called when the game starts or when spawned
void ASamuraiCharacter::BeginPlay()
{
	Super::BeginPlay();

	Instance = GetMesh()->GetAnimInstance();

	SamuraiController = Cast<ASamuraiController>(GetController());

	CombatBox->OnComponentBeginOverlap.AddDynamic(this, &ASamuraiCharacter::OnCombatBoxOverlapBegin);
	CombatBox->OnComponentEndOverlap.AddDynamic(this, &ASamuraiCharacter::OnCombatBoxOverlapEnd);

	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &ASamuraiCharacter::OnDetectSphereOverlapBegin);
	DetectSphere->OnComponentEndOverlap.AddDynamic(this, &ASamuraiCharacter::OnDetectSphereOverlapEnd);

	DetectSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DetectSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called every frame
void ASamuraiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if (SamuraiController && TargetEnemy)
	{
		SamuraiController->UpdateAIHealthBarPosition(TargetEnemy->GetActorLocation());
	}*/
	float Delta = DeltaTime * TenacityRecoverRate;
	if (!IsAttacking1 && !IsAttacking2 && !IsEvading)
	{
		Tenacity += Delta;
		if (Tenacity > MaxTenacity)
		{
			Tenacity = MaxTenacity;
			//	IsExhausted = false;
		}
	}

}

// Called to bind functionality to input
void ASamuraiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//	PlayerInputComponent->BindAxis("MoveForward", this, &ASamuraiCharacter::MoveForward);
	//	PlayerInputComponent->BindAxis("MoveRight", this, &ASamuraiCharacter::MoveRight);

		//PlayerInputComponent->BindAxis("Turn", this, &ASamuraiCharacter::AddControllerYawInput);
		//PlayerInputComponent->BindAxis("LookUp", this, &ASamuraiCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Katana", IE_Pressed, this, &ASamuraiCharacter::Katana);
	PlayerInputComponent->BindAction("Saber", IE_Pressed, this, &ASamuraiCharacter::Saber);
	PlayerInputComponent->BindAction("Archer", IE_Pressed, this, &ASamuraiCharacter::Archer);
	PlayerInputComponent->BindAction("Lancer", IE_Pressed, this, &ASamuraiCharacter::Lancer);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASamuraiCharacter::Jumping);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASamuraiCharacter::StopJump);

	PlayerInputComponent->BindAction("Attack1", IE_Pressed, this, &ASamuraiCharacter::Attack1);
	PlayerInputComponent->BindAction("Attack2", IE_Pressed, this, &ASamuraiCharacter::Attack2);

	PlayerInputComponent->BindAction("Defense", IE_Pressed, this, &ASamuraiCharacter::Defense);
	PlayerInputComponent->BindAction("Defense", IE_Released, this, &ASamuraiCharacter::DefenseRelease);

	PlayerInputComponent->BindAction("Attack1", IE_Released, this, &ASamuraiCharacter::AttackReleased1);

	//PlayerInputComponent->BindAction("Evade", IE_Pressed, this, &ASamuraiCharacter::Evade);

//	PlayerInputComponent->BindAction("Menu", IE_Pressed, this, &ASamuraiCharacter::TogglePause);

	//PlayerInputComponent->BindAction("Recover", IE_Pressed, this, &ASamuraiCharacter::Recover);
}
/*
void ASamuraiCharacter::Recover()
{
	if (Health == MaxHealth)
	{
		return;
	}
	else if (NumOfRecover == 0)
	{
		return;
	}
	else {
		NumOfRecover -= 1;
		Health += RecoverHP;
		if (Health > MaxHealth)
		{
			Health = MaxHealth;
		}
		UGameplayStatics::SpawnEmitterAtLocation(this, RecoverParticle, GetMesh()->GetRelativeLocation());
	//	UGameplayStatics::SpawnEmitterAtLocation(this, Man->InteractParticle, GetMesh()->GetSocketLocation("katana3"));
	}
}*/
/*
void ASamuraiCharacter::MoveForward(float Value)
{
	if (!IsAttacking1 && !IsAttacking2)
	{
		// 找出"前进"方向，并记录玩家想向该方向移动。
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
//		AxisX = Value;
	}

}

void ASamuraiCharacter::MoveRight(float Value)
{
	if (!IsAttacking1 && !IsAttacking2)
	{
		// 找出"右侧"方向，并记录玩家想向该方向移动。
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	//	AxisY = Value;
	}
}*/

void ASamuraiCharacter::Jumping()
{
	//UE_LOG(LopTemp, Log, TEXT("Jump Evade %d"), IsEvading);
	if (!IsDefensed && !IsDead && !IsAttacking1 && !IsEvading && !Instance->Montage_IsPlaying(Equip) && !Instance->Montage_IsPlaying(Unequip) && !IsAttacking2 && CurrentAttack == 0)
	{
		//IsInAir = true;
		ACharacter::Jump();
	}
}

void ASamuraiCharacter::StopJump()
{
	//if (!IsAttacking1 && !IsEvading && !Instance->Montage_IsPlaying(Equip) && !Instance->Montage_IsPlaying(Unequip)&& !IsAttacking2)
	//{
	//IsInAir = false;

	ACharacter::StopJumping();
	//}
}

void ASamuraiCharacter::FocusOn()
{

}

void ASamuraiCharacter::Katana()
{
	if (!IsDead)
	{
		//FPlatformProcess::Sleep(2.0f);
		if (CurrentStatus == EStatus::ES_Katana && !IsAttacking1 && !IsEvading && !GetMovementComponent()->IsFalling())
		{

			PlayAnimMontage(Unequip, SpeedRatio);
			//  katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("scabbard1"));

		}
		else if (CurrentStatus == EStatus::ES_Saber && !GetMovementComponent()->IsFalling() && !IsAttacking1 && !IsEvading && !IsAttacking2)
		{
			SetStatus(EStatus::ES_Katana);
			//PlayAnimMontage(Equip, SpeedRatio);
			//katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("katana3"));
		}
		else if (CurrentStatus == EStatus::ES_Archer && !GetMovementComponent()->IsFalling() && !IsAttacking1 && !IsEvading && !IsAttacking2)
		{
			SetStatus(EStatus::ES_Katana);
			katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("katana3"));
			Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bow"));
		}
		else if (CurrentStatus == EStatus::ES_Normal && !GetMovementComponent()->IsFalling() && !IsAttacking1 && !IsEvading && !IsAttacking2)
		{
			PlayAnimMontage(Equip, SpeedRatio);
		}


	}



}

void ASamuraiCharacter::Saber()
{
	if (!IsDead)
	{
		if (CurrentStatus == EStatus::ES_Saber && !IsAttacking1 && !IsEvading && !GetMovementComponent()->IsFalling())
		{

			PlayAnimMontage(Unequip, SpeedRatio);
			//  katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("scabbard1"));


		}
		else if (CurrentStatus == EStatus::ES_Katana && !IsAttacking1 && !IsEvading && !GetMovementComponent()->IsFalling())
		{
			SetStatus(EStatus::ES_Saber);
			//katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("katana3"));
		}
		else if (CurrentStatus == EStatus::ES_Archer && !GetMovementComponent()->IsFalling() && !IsAttacking1 && !IsEvading && !IsAttacking2)
		{
			SetStatus(EStatus::ES_Saber);
			katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("katana3"));
			Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bow"));
		}
		else if (CurrentStatus == EStatus::ES_Normal && !GetMovementComponent()->IsFalling() && !IsAttacking1 && !IsEvading && !IsAttacking2 && !Instance->Montage_IsPlaying(Equip))
		{
			PlayAnimMontage(Equip, SpeedRatio);
			SetStatus(EStatus::ES_Saber);
		}

	}

}

void ASamuraiCharacter::Lancer()
{

}

void ASamuraiCharacter::Archer()
{
	if (!IsDead && !IsFocusOn)
	{
		if (CurrentStatus == EStatus::ES_Saber && !IsAttacking1 && !IsEvading && !GetMovementComponent()->IsFalling())
		{

			SetStatus(EStatus::ES_Archer);

			katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("scabbard1"));
			Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LeftHandWeapon"));


		}
		else if (CurrentStatus == EStatus::ES_Katana && !IsAttacking1 && !IsEvading && !GetMovementComponent()->IsFalling())
		{
			SetStatus(EStatus::ES_Archer);

			katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("scabbard1"));
			Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LeftHandWeapon"));
		}
		else if (CurrentStatus == EStatus::ES_Archer && !GetMovementComponent()->IsFalling() && !IsAttacking1 && !IsEvading && !IsAttacking2)
		{
			SetStatus(EStatus::ES_Normal);
			Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bow"));
		}
		else if (CurrentStatus == EStatus::ES_Normal && !GetMovementComponent()->IsFalling() && !IsAttacking1 && !IsEvading && !IsAttacking2)
		{
			SetStatus(EStatus::ES_Archer);
			Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LeftHandWeapon"));
		}

	}
}

void ASamuraiCharacter::InAirBegin()
{
	IsInAir = true;
}

void ASamuraiCharacter::InAirEnd()
{
	IsInAir = false;
}

void ASamuraiCharacter::Attack1()
{
	if (!IsDead && !IsDefensed && !Instance->Montage_IsPlaying(Collide) && !Instance->Montage_IsPlaying(CollideFront))
	{
		if (TargetEnemy)
		{
			FRotator CurrentRotation = GetActorRotation();
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetEnemy->GetActorLocation());
			TargetRotation = FRotator(CurrentRotation.Pitch, TargetRotation.Yaw, CurrentRotation.Roll);
			SetActorRotation(TargetRotation);
		}
		if (CurrentStatus == EStatus::ES_Katana)
		{
			if (!IsAttacking1 && !IsInAir && !IsEvading && !GetMovementComponent()->IsFalling() && Tenacity >= 5)
			{
				IsAttacking1 = true;
				IsAttacking2 = false;
				IsAttacking = true;
				Tenacity -= 5;
				katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("katana3"));
				PlayAnimMontage(KatanaMontage[CurrentAttack], SpeedRatio);
			}

		}
		else if (CurrentStatus == EStatus::ES_Saber)
		{
			if (!IsAttacking1 && !IsInAir && !IsEvading && !GetMovementComponent()->IsFalling() && Tenacity >= 5)
			{
				IsAttacking1 = true;
				IsAttacking2 = false;
				IsAttacking = true;
				Tenacity -= 5;
				katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("katana3"));
				PlayAnimMontage(SaberMontage[CurrentAttack], SpeedRatio);
			}
		}
		else if (CurrentStatus == EStatus::ES_Archer)
		{
			if (NumOfArrow > 0)
			{
				if (!IsAttacking1 && !IsEvading && !GetMovementComponent()->IsFalling() && !IsAttacking2 && Tenacity >= 5)
				{
					SpringArmComp->SetRelativeLocation(FVector(0.0f, 100.0f, 77.0f));

					IsAttacking1 = true;
					IsAttacking = true;
					IsPrepared = true;
					bUseControllerRotationYaw = true;
					PlayAnimMontage(PrepareToShoot, SpeedRatio);

				}

			}

		}

	}
}


void ASamuraiCharacter::AttackReleased1()
{
	if (CurrentStatus == EStatus::ES_Archer && !Instance->Montage_IsPlaying(PrepareToShoot))
	{
		if (IsPrepared == true)
		{
			IsAttacking1 = false;
			IsAttacking = false;
			IsPrepared = false;
			bUseControllerRotationYaw = false;
			NumOfArrow -= 1;
			Tenacity -= 5;
			if (ProjectileClass != nullptr)
			{
				UWorld* const World = GetWorld();
				if (World != nullptr)
				{
					const FRotator SpawnRotation = GetControlRotation();
					// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
					const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					// spawn the projectile at the muzzle
					World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

				}
			}
		}
	}
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 77.0f));

}


void ASamuraiCharacter::Attack2()
{
	if (!IsDead && !IsDefensed && !Instance->Montage_IsPlaying(Collide) && !Instance->Montage_IsPlaying(CollideFront))
	{
		if (TargetEnemy)
		{
			FRotator CurrentRotation = GetActorRotation();
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetEnemy->GetActorLocation());
			TargetRotation = FRotator(CurrentRotation.Pitch, TargetRotation.Yaw, CurrentRotation.Roll);
			SetActorRotation(TargetRotation);
		}
		if (CurrentStatus == EStatus::ES_Katana || CurrentStatus == EStatus::ES_Normal)
		{
			if (!IsAttacking1 && !IsInAir && !IsEvading && !IsAttacking2 && !GetMovementComponent()->IsFalling() && Tenacity >= 10)
			{
				IsAttacking2 = true;
				IsAttacking = true;
				IsPrepared = false;
				Tenacity -= 10;
				katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("scabbard1"));
				SetStatus(EStatus::ES_Normal);

				PlayAnimMontage(KatanaHeavy, SpeedRatio);
			}

		}
		else if (CurrentStatus == EStatus::ES_Saber)
		{
			if (!IsAttacking1 && !IsInAir && !IsEvading && !IsAttacking2 && !GetMovementComponent()->IsFalling() && Tenacity >= 10)
			{
				IsAttacking2 = true;
				IsAttacking = true;
				Tenacity -= 10;
				katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("katana3"));
				PlayAnimMontage(SaberHeavy, SpeedRatio);

			}
		}
		else if (CurrentStatus == EStatus::ES_Archer)
		{
			IsAttacking1 = false;
			IsAttacking = false;
			IsPrepared = false;
			bUseControllerRotationYaw = false;
		}

	}
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 77.0f));
}

void ASamuraiCharacter::SetCurrentAttack()
{
	CurrentAttack = 0;
	IsAttacking = false;
}



void ASamuraiCharacter::AttackEnd1()
{
	CurrentAttack += 1;
	if (CurrentAttack == 4)
	{
		CurrentAttack = 0;
	}
	if (AttackSound)
	{
		UGameplayStatics::PlaySound2D(this, AttackSound);
	}
	IsAttacking1 = false;
}

void ASamuraiCharacter::AttackEnd2()
{
	if (AttackSound)
	{
		UGameplayStatics::PlaySound2D(this, AttackSound);
	}
	IsAttacking2 = false;
	CurrentAttack = 0;
}

void ASamuraiCharacter::AttackBegin1()
{

}

void ASamuraiCharacter::AttackBegin2()
{

}


void ASamuraiCharacter::Evade()
{
	if (!IsDead)
	{
		if (!IsEvading && !IsInAir && !GetMovementComponent()->IsFalling())
		{
			float temp = Tenacity - MaxTenacity * TenacityConsumeRate;
			if (temp > 0)
			{
				Tenacity -= MaxTenacity * TenacityConsumeRate;
				PlayAnimMontage(Roll, SpeedRatio);
			}
		}

	}
}

void ASamuraiCharacter::SetEvading()
{
	IsEvading = true;
	IsAttacking1 = false;
	IsAttacking2 = false;

}

void ASamuraiCharacter::StopEvade()
{
	IsEvading = false;
}



void ASamuraiCharacter::SufferDamage(float Dmg)
{
	float temp = Health - Dmg;
	if (temp <= 0)
	{
		Health = 0;
		Die();
		return;
	}

	Health = temp;
	float tempT = Tenacity - 10;
	SufferAccount += 1;
	if (tempT < 0 || SufferAccount == MaxSufferAccount)
	{
		if (tempT < 0)
		{
			Tenacity = 0;
		}
		if (SufferAccount == MaxSufferAccount)
		{
			SufferAccount = 0;
		}
		if (!GetMovementComponent()->IsFalling())
		{
			PlayAnimMontage(Collide);

			IsEvading = false;
			IsAttacking = false;
			IsAttacking2 = false;
			IsAttacking1 = false;
			IsDefensed = false;
		}

	}
	else {
		Tenacity = tempT;
	}


}

void ASamuraiCharacter::Die()
{
	IsDead = true;
	PlayAnimMontage(Death);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float ASamuraiCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	AStandardAI* AI = Cast<AStandardAI>(DamageCauser);
	if (AI)
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator EnemyRotation = DamageCauser->GetActorRotation();
		float Mul = CurrentRotation.Pitch * EnemyRotation.Pitch + CurrentRotation.Yaw * EnemyRotation.Yaw + CurrentRotation.Roll * EnemyRotation.Roll;
		//	float MulRight = Mul + 90.0f * EnemyRotation.Roll;
		if (IsDefensed && Mul < 0)
		{
			float tempT = Tenacity - 10;
			if (tempT < 0)
			{
				Tenacity = 0;
				PlayAnimMontage(Collide);
				IsEvading = false;
				IsAttacking = false;
				IsAttacking2 = false;
				IsAttacking1 = false;

			}
			else {
				Tenacity = tempT;
			}
			return 0;
		}
		else {
			float temp = Health - DamageAmount;
			if (temp <= 0)
			{
				Health = 0;
				Die();
				return DamageAmount;
			}

			Health = temp;
			float tempT = Tenacity - 10;
			SufferAccount += 1;
			if (tempT < 0 || SufferAccount == MaxSufferAccount)
			{
				if (tempT < 0)
				{
					Tenacity = 0;
				}
				if (SufferAccount == MaxSufferAccount)
				{
					SufferAccount = 0;
				}
				if (!GetMovementComponent()->IsFalling())
				{
					if (Mul <= 0)
					{
						PlayAnimMontage(Collide);
					}
					else
					{
						PlayAnimMontage(CollideFront);
					}

					IsEvading = false;
					IsAttacking = false;
					IsAttacking2 = false;
					IsAttacking1 = false;
					IsDefensed = false;
				}

			}
			else {
				Tenacity = tempT;
			}
			return DamageAmount;
		}
	}
	else {
		FRotator CurrentRotation = GetActorRotation();
		FRotator EnemyRotation = DamageCauser->GetActorRotation();
		float Mul = CurrentRotation.Pitch * EnemyRotation.Pitch + CurrentRotation.Yaw * EnemyRotation.Yaw + CurrentRotation.Roll * EnemyRotation.Roll;
		float temp = Health - DamageAmount;
		if (temp <= 0)
		{
			Health = 0;
			Die();
			return DamageAmount;
		}

		Health = temp;
		float tempT = Tenacity - 10;
		SufferAccount += 1;
		if (tempT < 0 || SufferAccount == MaxSufferAccount)
		{
			if (tempT < 0)
			{
				Tenacity = 0;
			}
			if (SufferAccount == MaxSufferAccount)
			{
				SufferAccount = 0;
			}
			if (Mul <= 0)
			{
				PlayAnimMontage(Collide);
			}
			else
			{
				PlayAnimMontage(CollideFront);
			}

			IsEvading = false;
			IsAttacking = false;
			IsAttacking2 = false;
			IsAttacking1 = false;
			IsDefensed = false;

		}
		else {
			Tenacity = tempT;
		}
		return DamageAmount;
	}
}
/*
void ASamuraiCharacter::DeathEnd()
{
	Instance->Montage_Pause();
}*/


void ASamuraiCharacter::SetStatus(EStatus Status)
{
	CurrentStatus = Status;
	switch (CurrentStatus)
	{
	case EStatus::ES_Normal:
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		break;
	case EStatus::ES_Katana:
		GetCharacterMovement()->MaxWalkSpeed = EquipSpeed;
		break;
	case EStatus::ES_Archer:
		GetCharacterMovement()->MaxWalkSpeed = EquipSpeed;
	default:
		break;
	}
}

void ASamuraiCharacter::PutToKatana()
{
	if (!IsDead)
	{
		katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("katana3"));
		SetStatus(EStatus::ES_Katana);
	}


}
void ASamuraiCharacter::PutToSaber()
{
	if (!IsDead)
	{
		katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("katana3"));
		SetStatus(EStatus::ES_Saber);

	}
}

void ASamuraiCharacter::PutToScabbard()
{
	if (!IsDead)
	{
		katana3->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("scabbard1"));
		SetStatus(EStatus::ES_Normal);
	}


}


void ASamuraiCharacter::OnCombatBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		//AAICharacter* Enemy = Cast<AAICharacter>(OtherActor);
		AStandardAI* Enemy = Cast<AStandardAI>(OtherActor);
		if (Enemy)
		{
			//ASamuraiCharacter* Man = Cast<ASamuraiCharacter>(GetOwner());
			//if (Man)
		//	{
			if (Enemy->InteractParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, Enemy->InteractParticle, GetMesh()->GetSocketLocation("katana3"));
			}
			if (Enemy->ReactSound)
			{
				UGameplayStatics::PlaySound2D(this, Enemy->ReactSound);
			}
			if (DamageTypeClass)
			{
				if (Instance->Montage_IsPlaying(KatanaHeavy) || Instance->Montage_IsPlaying(SaberHeavy))
				{
					UGameplayStatics::ApplyDamage(Enemy, KatanaHeavyDamage, this->GetController(), this, DamageTypeClass);
				}
				else
				{
					UGameplayStatics::ApplyDamage(Enemy, KatanaDamage, this->GetController(), this, DamageTypeClass);
				}
			}
			//}
		}
	}

}

void ASamuraiCharacter::OnCombatBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


void ASamuraiCharacter::EnableWeaponCollision()
{
	CombatBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASamuraiCharacter::DisableWeaponCollision()
{
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ASamuraiCharacter::PlayAttackSound()
{
	if (AttackSound)
	{
		UGameplayStatics::PlaySound2D(this, AttackSound);
	}
}

void ASamuraiCharacter::OnDetectSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!TargetEnemy)
	{
		if (OtherActor)
		{
			AStandardAI* Enemy = Cast<AStandardAI>(OtherActor);
			if (Enemy)
			{
				TargetEnemy = Enemy;

			}
		}
	}
}

void ASamuraiCharacter::OnDetectSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AStandardAI* Enemy = Cast<AStandardAI>(OtherActor);
		if (Enemy == TargetEnemy)
		{
			TargetEnemy = nullptr;

		}
	}
}

void ASamuraiCharacter::Defense()
{
	IsDefensed = true;

	//	PlayAnimMontage(PrepareToDefense, SpeedRatio);
}

void ASamuraiCharacter::DefenseRelease()
{
	IsDefensed = false;

	//	PlayAnimMontage(CancelDefense, SpeedRatio);
}

//以下是lock部分
