// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SamuraiController.generated.h"

/**
 *
 */
UCLASS()
class SAMURAI_API ASamuraiController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> HUDAsset;

	UUserWidget* HUD;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> AIHealthBarAsset;

	UUserWidget* AIHealthBar;

	void SetAIHealthBarVisibility(bool Visibility);

	void UpdateAIHealthBarPosition(FVector Position);
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> PauseMenuAsset;

	UUserWidget* PauseMenu;

	void SetPauseMenuVisibility(bool Visibility);

	bool bPause;

	void TogglePause();

protected:
	virtual void BeginPlay() override;

};
