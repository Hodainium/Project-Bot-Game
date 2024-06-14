// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/LyraGameplayAbilityTargetData_SingleTargetHit.h"
#include "PBTargetDataTypes.generated.h"

/**
 * 
 */
 /** Target data with just a source and target location in space */
USTRUCT(BlueprintType)
struct FGameplayAbilityTargetData_PBMeleeInput : public FGameplayAbilityTargetData
{
	GENERATED_USTRUCT_BODY()

	/** Combo number */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Targeting)
	uint8 Combo;

	/** Angle of attack */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Targeting)
	float AttackAngle;

	// -------------------------------------

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayAbilityTargetData_PBMeleeInput::StaticStruct();
	}

	virtual FString ToString() const override
	{
		return TEXT("FGameplayAbilityTargetData_PBMeleeInput");
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_PBMeleeInput> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_PBMeleeInput>
{
	enum
	{
		WithNetSerializer = true	// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};



/** Target data with just a source and target location in space */
USTRUCT(BlueprintType)
struct FPBGameplayAbilityTargetData_MeleeHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_USTRUCT_BODY()

	/** Combo number */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Targeting)
	uint8 Combo;

	// -------------------------------------

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FPBGameplayAbilityTargetData_MeleeHit::StaticStruct();
	}

	virtual FString ToString() const override
	{
		return TEXT("FPBGameplayAbilityTargetData_MeleeHit");
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FPBGameplayAbilityTargetData_MeleeHit> : public TStructOpsTypeTraitsBase2<FPBGameplayAbilityTargetData_MeleeHit>
{
	enum
	{
		WithNetSerializer = true	// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};
