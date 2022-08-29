// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SamuraiAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class SAMURAI_API USamuraiAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float AxisX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float AxisY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float CurrentAxisX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float CurrentAxisY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		bool IsEvading;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		bool IsFocusOn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		bool IsDefensed;

	//virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperty)
		void UpdateAnimationProperties();
	/*
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class ASamuraiCharacter* SamuraiMan;
		*/
};
