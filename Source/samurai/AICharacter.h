// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StandardAI.h"
#include "AICharacter.generated.h"

UENUM(BlueprintType)
enum class EMoveStatus :uint8
{
	MS_Idle         UMETA(DisplayName = "Idle"),
	MS_MoveToTarget UMETA(DisplayName = "MoveToTarget"),
	MS_Attacking    UMETA(DisplayName = "Attacking"),
	MS_Dead         UMETA(DisplayName = "Dead"),
};

UCLASS()
class SAMURAI_API AAICharacter : public AStandardAI
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EMoveStatus MoveStatus;

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
		UAnimMontage* Hitted;

	//int32 HitAccount;
	//int32 MaxHitAccount;

	UPROPERTY(EditAnyWhere, Category = "Animation")
		UAnimMontage* Death;
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

	class AAIController* AIController;

	UFUNCTION(BlueprintCallable)
		void OnCombatBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
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
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser)override;
*/
	UFUNCTION(BlueprintCallable)
		void Die();
	/*
		UFUNCTION(BlueprintCallable)
			void DeathEnd();
			*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType>DamageTypeClass;

};
