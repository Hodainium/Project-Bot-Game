// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PBItemSearchQuery.generated.h"

enum class EPBItemQuality : uint8;
/**
 * 
 */
USTRUCT(BlueprintType)
struct LYRAGAME_API FPBItemSearchQuery
{
	GENERATED_BODY()

	FPBItemSearchQuery()
	{}

public:
	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer RequiredTags;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer BlockedTags;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer AccumulatedTags;

	UPROPERTY(BlueprintReadWrite)
	TSet<EPBItemQuality> BlockedModQualities;

	UPROPERTY(BlueprintReadWrite)
	TSet<FName> BlockedAssetNames;
};
