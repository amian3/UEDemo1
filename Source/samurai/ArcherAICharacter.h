// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StandardAI.h"
#include "ArcherAICharacter.generated.h"

UENUM(BlueprintType)
enum class EArcherMoveStatus :uint8
{
	MS_Idle         UMETA(DisplayName = "Idle"),
	MS_MoveToTarget UMETA(DisplayName = "MoveToTarget"),
	MS_Attacking    UMETA(DisplayName = "Attacking"),
	MS_Dead         UMETA(DisplayName = "Dead"),
};

UCLASS()
class SAMURAI_API AArcherAICharacter : public AStandardAI
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AArcherAICharacter();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EArcherMoveStatus MoveStatus;

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
		class UWidgetComponent* HPBar;
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Cone;
*/
	class AAIController* AIController;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundCue* AttackSound;
		*/
	UFUNCTION(BlueprintCallable)
		void Die();
	/*
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			float Health;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			float MaxHealth;*/


};
