// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StandardAI.h"
#include "NormalAICharacter.generated.h"

UENUM(BlueprintType)
enum class ENormalMoveStatus :uint8
{
	MS_Idle         UMETA(DisplayName = "Idle"),
	MS_MoveToTarget UMETA(DisplayName = "MoveToTarget"),
	MS_Attacking    UMETA(DisplayName = "Attacking"),
	MS_Dead         UMETA(DisplayName = "Dead"),
};

UCLASS()
class SAMURAI_API ANormalAICharacter : public AStandardAI
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANormalAICharacter();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ENormalMoveStatus MoveStatus;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnyWhere)
		UStaticMeshComponent* Weapon;

	UAnimInstance* Instance;

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* AnimMontage[4];

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* Death;

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* Collide;
	/*
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
			UParticleSystem* InteractParticle;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			class USoundCue* ReactSound;
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* CombatBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWidgetComponent* HPBar;
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Cone;
*/
	class AAIController* AIController;

	UFUNCTION()
		void OnCombatBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnCombatBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundCue* AttackSound;
		*/
	UFUNCTION(BlueprintCallable)
		void EnableCollision();

	UFUNCTION(BlueprintCallable)
		void DisableCollision();
	/*
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser)override;
	*/
	UFUNCTION(BlueprintCallable)
		void Die();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType>DamageTypeClass;
	/*
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			float Health;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			float MaxHealth;*/
};
