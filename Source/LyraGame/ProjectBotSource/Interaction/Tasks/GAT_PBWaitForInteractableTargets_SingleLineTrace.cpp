// Fill out your copyright notice in the Description page of Project Settings.


#include "GAT_PBWaitForInteractableTargets_SingleLineTrace.h"
#include "ProjectBotSource/Interaction/PBInteractionStatics.h"

UGAT_PBWaitForInteractableTargets_SingleLineTrace::UGAT_PBWaitForInteractableTargets_SingleLineTrace(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UGAT_PBWaitForInteractableTargets_SingleLineTrace* UGAT_PBWaitForInteractableTargets_SingleLineTrace::WaitForInteractableTargets_SingleLineTrace(UGameplayAbility* OwningAbility, FPBInteractionQuery InteractionQuery, FCollisionProfileName TraceProfile, FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionScanRange, float InteractionScanRate, bool bShowDebug)
{
	UGAT_PBWaitForInteractableTargets_SingleLineTrace* MyObj = NewAbilityTask<UGAT_PBWaitForInteractableTargets_SingleLineTrace>(OwningAbility);
	MyObj->InteractionScanRange = InteractionScanRange;
	MyObj->InteractionScanRate = InteractionScanRate;
	MyObj->StartLocation = StartLocation;
	MyObj->InteractionQuery = InteractionQuery;
	MyObj->TraceProfile = TraceProfile;
	MyObj->bShowDebug = bShowDebug;

	return MyObj;
}

void UGAT_PBWaitForInteractableTargets_SingleLineTrace::Activate()
{
	SetWaitingOnAvatar();

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PerformTrace, InteractionScanRate, true);
}

void UGAT_PBWaitForInteractableTargets_SingleLineTrace::OnDestroy(bool AbilityEnded)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::OnDestroy(AbilityEnded);
}

void UGAT_PBWaitForInteractableTargets_SingleLineTrace::PerformTrace()
{
	AActor* AvatarActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
	if (!AvatarActor)
	{
		return;
	}

	UWorld* World = GetWorld();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	const bool bTraceComplex = false;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UGAT_PBWaitForInteractableTargets_SingleLineTrace), bTraceComplex);
	Params.AddIgnoredActors(ActorsToIgnore);

	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
	FVector TraceEnd;
	AimWithPlayerController(AvatarActor, Params, TraceStart, InteractionScanRange, OUT TraceEnd);

	FHitResult OutHitResult;
	LineTrace(OutHitResult, World, TraceStart, TraceEnd, TraceProfile.Name, Params);

	TArray<TScriptInterface<IPBInteractableTarget>> InteractableTargets;
	UPBInteractionStatics::AppendInteractableTargetsFromHitResult(OutHitResult, InteractableTargets);

	UpdateInteractableOptions(InteractionQuery, InteractableTargets);

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FColor DebugColor = OutHitResult.bBlockingHit ? FColor::Red : FColor::Green;
		if (OutHitResult.bBlockingHit)
		{
			DrawDebugLine(World, TraceStart, OutHitResult.Location, DebugColor, false, InteractionScanRate);
			DrawDebugSphere(World, OutHitResult.Location, 5, 16, DebugColor, false, InteractionScanRate);
		}
		else
		{
			DrawDebugLine(World, TraceStart, TraceEnd, DebugColor, false, InteractionScanRate);
		}
	}
#endif // ENABLE_DRAW_DEBUG
}