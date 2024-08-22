// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectBotSource/Interaction/PBInteractionQuery.h"
#include "GAT_PBWaitForInteractableTargets.h"
#include "GAT_PBWaitForInteractableTargets_SingleLineTrace.generated.h"

struct FCollisionProfileName;

class UGameplayAbility;
class UObject;
struct FFrame;

UCLASS()
class UGAT_PBWaitForInteractableTargets_SingleLineTrace : public UGAT_PBWaitForInteractableTargets
{
	GENERATED_UCLASS_BODY()

	virtual void Activate() override;

	/** Wait until we trace new set of interactables.  This task automatically loops. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UGAT_PBWaitForInteractableTargets_SingleLineTrace* WaitForInteractableTargets_SingleLineTrace(UGameplayAbility* OwningAbility, FPBInteractionQuery InteractionQuery, FCollisionProfileName TraceProfile, FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionScanRange = 100, float InteractionScanRate = 0.100, bool bShowDebug = false);

private:

	virtual void OnDestroy(bool AbilityEnded) override;

	void PerformTrace();

	UPROPERTY()
	FPBInteractionQuery InteractionQuery;

	UPROPERTY()
	FGameplayAbilityTargetingLocationInfo StartLocation;

	float InteractionScanRange = 100;
	float InteractionScanRate = 0.100;
	bool bShowDebug = false;

	FTimerHandle TimerHandle;
};

