// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "Engine/CollisionProfile.h"
#include "ProjectBotSource/Interaction/PBInteractionOption.h"

#include "GAT_PBWaitForInteractableTargets.generated.h"

class AActor;
class IPBInteractableTarget;
class UObject;
class UWorld;
struct FCollisionQueryParams;
struct FHitResult;
struct FPBInteractionQuery;
template <typename InterfaceType> class TScriptInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPBInteractableObjectsChangedEvent, const TArray<FPBInteractionOption>&, InteractableOptions);

UCLASS(Abstract)
class UGAT_PBWaitForInteractableTargets : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FPBInteractableObjectsChangedEvent InteractableObjectsChanged;

protected:

	static void LineTrace(FHitResult& OutHitResult, const UWorld* World, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params);

	void AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, float MaxRange, FVector& OutTraceEnd, bool bIgnorePitch = false) const;

	static bool ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition);

	void UpdateInteractableOptions(const FPBInteractionQuery& InteractQuery, const TArray<TScriptInterface<IPBInteractableTarget>>& InteractableTargets);

	FCollisionProfileName TraceProfile;

	// Does the trace affect the aiming pitch
	bool bTraceAffectsAimPitch = true;

	TArray<FPBInteractionOption> CurrentOptions;
};
