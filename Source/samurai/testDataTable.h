// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "testDataTable.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTableRowTest : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FTableRowTest() {}

	FTableRowTest(bool InLoop, int32 InCurrentCount, int32 InMaxNum, float InLifeTime)
		: bLoop(InLoop)
		, CurrentCount(InCurrentCount)
		, MaxNum(InMaxNum)
		, LifeTime(InLifeTime)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "是否循环")
		bool		bLoop = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "当前数量")
		int64		CurrentCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "最大数量")
		int64		MaxNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "生命周期")
		float		LifeTime = 0.f;
};

UCLASS()
class SAMURAI_API UtestDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
