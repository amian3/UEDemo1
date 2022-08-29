// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StandardAI.generated.h"

UCLASS()
class SAMURAI_API AStandardAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStandardAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UParticleSystem* InteractParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* ReactSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Cone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHealth;
};
