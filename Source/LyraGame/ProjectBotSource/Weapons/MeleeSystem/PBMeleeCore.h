// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RamaMeleeCore.h"
#include "PBMeleeCore.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API UPBMeleeCore : public URamaMeleeCore
{
	GENERATED_BODY()

public:

	static bool MeleeSweepByChannel(
		URamaMeleeWeapon* Comp,
		UWorld* World,
		AActor* ActorToIgnore,
		FHitResult& Hit,
		const FVector& Start,
		const FVector& End,
		const FRamaMeleeShape& RamaMeleeShape,
		const ECollisionChannel TraceChannel,
		const FQuat& Rot = FQuat::Identity
	);

	static bool MeleeSweepByChannel(
		URamaMeleeWeapon* Comp,
		UWorld* World,
		TArray<AActor*> ActorsToIgnore,
		FHitResult& Hit,
		const FVector& Start,
		const FVector& End,
		const FRamaMeleeShape& RamaMeleeShape,
		const ECollisionChannel TraceChannel,
		const FQuat& Rot = FQuat::Identity
	);

	static FORCEINLINE bool MeleeSweepByChannel(
		URamaMeleeWeapon* Comp,
		UWorld* World,
		AActor* ActorToIgnore,
		FHitResult& Hit,
		const FMeleeSweepData& MeleeSweep,
		const ECollisionChannel TraceChannel
	) {
		return UPBMeleeCore::MeleeSweepByChannel(
			Comp,
			World,
			ActorToIgnore,
			Hit,
			MeleeSweep.Start,
			MeleeSweep.End,
			MeleeSweep,
			TraceChannel,
			MeleeSweep.Rotation
		);
	}
	
	
	static FORCEINLINE bool MeleeSweepByChannel(
		URamaMeleeWeapon* Comp,
		UWorld* World,
		TArray<AActor*> ActorsToIgnore,
		FHitResult& Hit,
		const FMeleeSweepData& MeleeSweep,
		const ECollisionChannel TraceChannel
	) {
		return UPBMeleeCore::MeleeSweepByChannel(
			Comp,
			World,
			ActorsToIgnore,
			Hit,
			MeleeSweep.Start,
			MeleeSweep.End,
			MeleeSweep,
			TraceChannel,
			MeleeSweep.Rotation
		);
	}
	
};
