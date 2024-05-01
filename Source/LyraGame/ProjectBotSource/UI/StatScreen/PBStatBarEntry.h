// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PBStatBarEntry.generated.h"

USTRUCT(BlueprintType)
struct LYRAGAME_API FPBStatBarPipData
{
	GENERATED_BODY()

	FPBStatBarPipData()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DefaultPips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TempPips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BonusPips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NegativePips;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FPBStatBarEntry
{
	GENERATED_BODY()

	FPBStatBarEntry(): StartTotalPips(0)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Categories = "UI.StatBar"))
	FGameplayTag EntryTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> RowIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int StartTotalPips;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPBStatBarPipData StartPipData;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FPBStatBarDefContainer
{
	GENERATED_BODY()

	FPBStatBarDefContainer()
	{}

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (ForceInlineRow, Categories = "UI.StatBar"), Category = StatBar)
	TMap<FGameplayTag, FPBStatBarEntry> StatBarEntries;
};
