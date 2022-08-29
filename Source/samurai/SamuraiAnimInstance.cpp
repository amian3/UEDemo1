// Fill out your copyright notice in the Description page of Project Settings.


#include "SamuraiAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SamuraiCharacter.h"
/*
void USamuraiAnimInstance::NativeInitializeAnimation()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
	SamuraiMan = Cast<ASamuraiCharacter>(Pawn);
}
*/

void USamuraiAnimInstance::UpdateAnimationProperties()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0);
		MovementSpeed = LateralSpeed.Size();

		IsInAir = Pawn->GetMovementComponent()->IsFalling();

		ASamuraiCharacter* Man = Cast<ASamuraiCharacter>(Pawn);
		AxisX = Man->AxisX;
		AxisY = Man->AxisY;
		CurrentAxisX = Man->CurrentAxisX;
		CurrentAxisY = Man->CurrentAxisY;
		IsEvading = Man->IsEvading;
		IsFocusOn = Man->IsFocusOn;
		IsDefensed = Man->IsDefensed;

	}

}