// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "PBStartingStatsAsset.generated.h"

class UPBStatDefinition;
class UGameplayEffect;
class ULyraAbilitySet;

/**
 * FPBInitialStat
 */

USTRUCT(BlueprintType)
struct LYRAGAME_API FPBInitialStat
{
	GENERATED_BODY()

	FPBInitialStat() : InitialStatLevel(0), InitialMaxStatLevel(0)
	{
	}

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPBStatDefinition> StatDef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 InitialStatLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 InitialMaxStatLevel;

	bool operator==(const FPBInitialStat& Other) const
	{
		return (StatDef == Other.StatDef) && (InitialStatLevel == Other.InitialStatLevel) && (InitialMaxStatLevel == Other.InitialMaxStatLevel);
	}
};


/**
 * UPBStatDefinition
 */

UCLASS(BlueprintType)
class LYRAGAME_API UPBStartingStatsAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPBStartingStatsAsset()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	TArray<FPBInitialStat> StartingStatLevels;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	FPBInitialStat PowerBankStartingStats;

private:

};
