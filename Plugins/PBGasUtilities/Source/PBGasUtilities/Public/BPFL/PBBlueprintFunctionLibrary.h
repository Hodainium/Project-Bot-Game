// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetDataFilter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PBBlueprintFunctionLibrary.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct PBGASUTILITIES_API FPBRemoveActorInstancesTargetDataFilter : public FGameplayTargetDataFilter
{
	GENERATED_BODY()

	/** Returns true if the actor passes the filter and will be targeted */
	virtual bool FilterPassesForActor(const AActor* ActorToBeFiltered) const override;

	/** Array of actor instances that the filter checks against. If set to reverse this will filter out actors that aren't in supplied array */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Filter|PB")
	TArray<TObjectPtr<AActor>> ActorsBlocked;
};

UCLASS()
class PBGASUTILITIES_API UPBBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintPure, Category = "Filter")
	static FGameplayTargetDataFilterHandle MakePBRemoveActorInstancesFilterHandle(FPBRemoveActorInstancesTargetDataFilter Filter, AActor* FilterActor);
};
