// Fill out your copyright notice in the Description page of Project Settings.


#include "BPFL/PBBlueprintFunctionLibrary.h"

#include "Logging/StructuredLog.h"


bool FPBRemoveActorInstancesTargetDataFilter::FilterPassesForActor(const AActor* ActorToBeFiltered) const
{
	if(!FGameplayTargetDataFilter::FilterPassesForActor(ActorToBeFiltered))
	{
		return (bReverseFilter ^ false);
	}

	return (!ActorsBlocked.Contains(ActorToBeFiltered) ^ bReverseFilter);
}

FGameplayTargetDataFilterHandle UPBBlueprintFunctionLibrary::MakePBRemoveActorInstancesFilterHandle(FPBRemoveActorInstancesTargetDataFilter Filter, AActor* FilterActor)
{
	FGameplayTargetDataFilter* NewFilter = new FPBRemoveActorInstancesTargetDataFilter(Filter);
	NewFilter->InitializeFilterContext(FilterActor);

	FGameplayTargetDataFilterHandle FilterHandle;
	FilterHandle.Filter = TSharedPtr<FGameplayTargetDataFilter>(NewFilter);
	return FilterHandle;
}