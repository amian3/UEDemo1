// Fill out your copyright notice in the Description page of Project Settings.


#include "SamuraiController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void ASamuraiController::BeginPlay()
{
	Super::BeginPlay();
	if (PauseMenuAsset)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, PauseMenuAsset);
	}
	if (PauseMenu)
	{
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		PauseMenu->AddToViewport();
	}


	if (HUDAsset)
	{
		HUD = CreateWidget<UUserWidget>(this, HUDAsset);
	}
	if (HUD)
	{
		HUD->AddToViewport();
	}
	/*
	if (AIHealthBarAsset)
	{
		AIHealthBar = CreateWidget<UUserWidget>(this, AIHealthBarAsset);
	}
	if (AIHealthBar)
	{
		AIHealthBar->SetVisibility(ESlateVisibility::Hidden);
		AIHealthBar->AddToViewport();
	}*/
}
/*
void ASamuraiController::SetAIHealthBarVisibility(bool Visibility)
{
	if (AIHealthBar)
	{
		if (Visibility)
		{
			AIHealthBar->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			AIHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


void ASamuraiController::UpdateAIHealthBarPosition(FVector Position)
{
	if (AIHealthBar)
	{
		FVector2D ScreenPosition;
		FVector TargetPosition = FVector(Position.X, Position.Y, Position.Z + 150);
		ProjectWorldLocationToScreen(TargetPosition, ScreenPosition);

		FVector2D ScreenSize(200, 25);
		ScreenPosition.X -= ScreenSize.X * 0.5f;
		AIHealthBar->SetPositionInViewport(ScreenPosition);
		AIHealthBar->SetDesiredSizeInViewport(ScreenSize);
	}
}*/

void ASamuraiController::SetPauseMenuVisibility(bool Visibility)
{
	if (PauseMenu)
	{
		if (Visibility)
		{
			PauseMenu->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ASamuraiController::TogglePause()
{
	bPause = !bPause;
	SetPauseMenuVisibility(bPause);
	bShowMouseCursor = bPause;
	UGameplayStatics::SetGamePaused(this, bPause);
}