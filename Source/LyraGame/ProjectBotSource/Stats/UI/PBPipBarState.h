// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PBPipBarState.generated.h"

USTRUCT(BlueprintType)
struct LYRAGAME_API FPBPipBarState
{
	GENERATED_BODY()

	FPBPipBarState(): TotalPips(0), EnabledPips(0), PoweredPips(0), TempPips(0), BonusPips(0), NegativePips(0)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TotalPips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnabledPips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PoweredPips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TempPips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BonusPips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NegativePips;
};
