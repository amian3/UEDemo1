#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SamuraiCharacter.generated.h"

//class USoundBase;

UENUM(BlueprintType)
enum class EStatus : uint8
{
	ES_Normal UMETA(DisplayName = "Normal"),
	ES_Katana UMETA(DisplayName = "Katana"),//是否持有武器
	ES_Saber  UMETA(DisplayName = "Saber"),
	ES_Archer UMETA(DisplayName = "Archer"),
	ES_Lancer UMETA(DisplayName = "Lancer"),
};
UCLASS()
class SAMURAI_API ASamuraiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASamuraiCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 处理用于前后移动的输入。
	//UFUNCTION()
	//	void MoveForward(float Value);

	// 处理用于左右移动的输入。
//	UFUNCTION()
	//	void MoveRight(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AxisX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AxisY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AxisZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentAxisX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentAxisY;

	UFUNCTION(BlueprintCallable)
		void Jumping();

	UFUNCTION(BlueprintCallable)
		void StopJump();

	UFUNCTION(BlueprintCallable)
		void InAirBegin();

	UFUNCTION(BlueprintCallable)
		void InAirEnd();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsFocusOn;

	UFUNCTION()
		void FocusOn();
public:
	class ASamuraiController* SamuraiController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EStatus CurrentStatus;

	void SetStatus(EStatus Status);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float RunSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float EquipSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Tenacity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float MaxTenacity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float TenacityRecoverRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float TenacityConsumeRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float DamageRatio;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float SpeedRatio;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float TenacityRatio;

	UFUNCTION()
		void Katana();

	UFUNCTION()
		void Saber();

	UFUNCTION()
		void Archer();

	UFUNCTION()
		void Lancer();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UCameraComponent* CameraComp;

	UPROPERTY(EditAnyWhere)
		UStaticMeshComponent* katana3;

	UPROPERTY(EditAnyWhere)
		UStaticMeshComponent* scabbard1;

	UPROPERTY(EditAnyWhere)
		UStaticMeshComponent* Bow;





	UFUNCTION(BlueprintCallable)
		void PutToKatana();

	UFUNCTION(BlueprintCallable)
		void PutToSaber();

	UFUNCTION(BlueprintCallable)
		void PutToScabbard();

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* KatanaMontage[4];

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* SaberMontage[4];
	/*
	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* ArcherMontage[4];

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* LancerMontage[4];
		*/
	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* KatanaHeavy;

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* SaberHeavy;
	/*
	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* ArcherHeavy;

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* LancerHeavy;
	*/
	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* Roll;

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* Equip;

	//UPROPERTY(EditAnyWhere, Category = "Animation")
		//UAnimMontage* EquipBow;

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* Unequip;

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* PrepareToShoot;

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* Death;

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* Collide;

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* CollideFront;
	/*
		UPROPERTY(EditAnyWhere, Category = "Animation")
			UAnimMontage* CollideRight;

		UPROPERTY(EditAnyWhere, Category = "Animation")
			UAnimMontage* CollideLeft;
	*/
	//	UPROPERTY(EditAnyWhere, Category = "Animation")
		//	UAnimMontage* PrepareToDefense;

	//	UPROPERTY(EditAnyWhere, Category = "Animation")
		//	UAnimMontage* CancelDefense;

	UAnimInstance* Instance;

	int SaveAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int CurrentAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsPrepared;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsPreparedInHeavy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 NumOfArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 ConsumeOfArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 HeavyConsumeOfArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 NumOfRecover;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float RecoverHP;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsDead;

	//	UFUNCTION(BlueprintCallable)
	//		void Recover();

	UFUNCTION(BlueprintCallable)
		void SetCurrentAttack();

	bool IsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float KatanaDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float KatanaHeavyDamage;





	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType> DamageTypeClass;

	UFUNCTION()
		void Attack1();

	UFUNCTION()
		void AttackReleased1();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsAttacking1;

	UFUNCTION()
		void Attack2();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsAttacking2;

	UFUNCTION(BlueprintCallable)
		void Evade();

	UFUNCTION(BlueprintCallable)
		void StopEvade();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsEvading;

	UFUNCTION(BlueprintCallable)
		void SetEvading();




	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AStandardAI* TargetEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* DetectSphere;

	UFUNCTION()
		void OnDetectSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnDetectSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SufferDamage(float Damage);

	int SufferAccount;
	int MaxSufferAccount;

	void Die();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser)override;
	/*
	UFUNCTION(BlueprintCallable)
		void DeathEnd();
	*/


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* CombatBox;

	UFUNCTION()
		void OnCombatBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnCombatBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* AttackSound;

	UFUNCTION(BlueprintCallable)
		void EnableWeaponCollision();

	UFUNCTION(BlueprintCallable)
		void DisableWeaponCollision();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UParticleSystem* InteractParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UParticleSystem* SteelParticle;
	/*
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UParticleSystem* RecoverParticle;
		*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* ReactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* SteelSound;

	UFUNCTION(BlueprintCallable)
		void PlayAttackSound();

	UFUNCTION(BlueprintCallable)
		void AttackEnd1();

	UFUNCTION(BlueprintCallable)
		void AttackEnd2();

	UFUNCTION(BlueprintCallable)
		void AttackBegin1();

	UFUNCTION(BlueprintCallable)
		void AttackBegin2();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AProjectile> ProjectileClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* FP_MuzzleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector GunOffset;

	UFUNCTION(BlueprintCallable)
		void Defense();

	UFUNCTION(BlueprintCallable)
		void DefenseRelease();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDefensed;

	//以下是lock部分


};

