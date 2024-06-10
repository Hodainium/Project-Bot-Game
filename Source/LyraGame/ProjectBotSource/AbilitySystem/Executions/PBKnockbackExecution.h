// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffectExecutionCalculation.h"

#include "PBKnockbackExecution.generated.h"

class UObject;


/**
 * UPBKnockbackExecution
 *
 *	Execution used by gameplay effects to apply healing to the health attributes.
 */
UCLASS()
class UPBKnockbackExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UPBKnockbackExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
