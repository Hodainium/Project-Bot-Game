// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "PBStatDefinition.generated.h"

class ULyraAbilitySet;

/**
 * FPBStatLevelEntry
 */

USTRUCT(BlueprintType)
struct LYRAGAME_API FPBStatLevelEntry
{
	GENERATED_BODY()

	FPBStatLevelEntry(): Cost(0)
	{
	}

	UPROPERTY(EditDefaultsOnly)
	int Cost;

	UPROPERTY(EditDefaultsOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<const ULyraAbilitySet>> AbilitySetsToGrant;
};


/**
 * UPBStatDefinition
 */

UCLASS(BlueprintType)
class LYRAGAME_API UPBStatDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPBStatDefinition(): MaxLevel(0)
	{
	}

	/** User-visible name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	FText StatName;

	/** Icon to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	TObjectPtr<UTexture2D> StatIcon;

	/** Max level for this stat */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	int MaxLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	FGameplayAttribute CurrentValueAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	FGameplayAttribute MaxValueAttribute;

	/** Stat Entries */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	TArray<FPBStatLevelEntry> StatEntries;

private:

};
