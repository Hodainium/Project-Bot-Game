// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Weapons/MeleeSystem/PBMeleeCore.h"

#include "RamaMeleeWeapon.h"


bool UPBMeleeCore::MeleeSweepByChannel(
	URamaMeleeWeapon* Comp,
	UWorld* World,
	AActor* ActorToIgnore,
	FHitResult& Hit,
	const FVector& Start,
	const FVector& End,
	const FRamaMeleeShape& RamaMeleeShape,
	const ECollisionChannel TraceChannel,
	const FQuat& Rot
) {
	if (!Comp || !World) return false;

	//Expose (not till multi threading solid tho)
	FCollisionQueryParams TraceParams(FName(TEXT("VictoreCore Sphere Sweep")), true, ActorToIgnore);
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(ActorToIgnore);

	//Physical Material
	TraceParams.bReturnPhysicalMaterial = true;

	//Initial Overlaps
	TraceParams.bFindInitialOverlaps = Comp->HitEvents_IncludeInitialOverlapsDuringSwingStart;

	return World->SweepSingleByChannel(
		Hit,
		Start,
		End,
		Rot,
		TraceChannel,
		RamaMeleeShape,
		TraceParams
	);
}

bool UPBMeleeCore::MeleeSweepByChannel(
	URamaMeleeWeapon* Comp,
	UWorld* World,
	TArray<AActor*> ActorsToIgnore,
	FHitResult& Hit,
	const FVector& Start,
	const FVector& End,
	const FRamaMeleeShape& RamaMeleeShape,
	const ECollisionChannel TraceChannel,
	const FQuat& Rot
) {
	if (!Comp || !World) return false;

	//Expose (not till multi threading solid tho)
	FCollisionQueryParams TraceParams(FName(TEXT("VictoreCore Sphere Sweep")), true); //complex
	TraceParams.bTraceComplex = true;

	for (AActor* Each : ActorsToIgnore)
	{
		TraceParams.AddIgnoredActor(Each);
	}

	//Physical Material
	TraceParams.bReturnPhysicalMaterial = true;

	//Initial Overlaps
	TraceParams.bFindInitialOverlaps = Comp->HitEvents_IncludeInitialOverlapsDuringSwingStart;

	return World->SweepSingleByChannel(
		Hit,
		Start,
		End,
		Rot,
		TraceChannel,
		RamaMeleeShape,
		TraceParams
	);
}

