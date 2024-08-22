// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPFL_PBTargetTypes.generated.h"

struct FGameplayAbilityTargetDataHandle;
/**
 * 
 */
UCLASS()
class LYRAGAME_API UBPFL_PBTargetTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure)
	static int GetComboNumFromTargetHandle(const FGameplayAbilityTargetDataHandle& Handle, const int Index);
};
