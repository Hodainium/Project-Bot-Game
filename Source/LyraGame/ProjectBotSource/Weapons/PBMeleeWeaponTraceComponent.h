// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RamaMeleeWeapon.h"
#include "PBMeleeWeaponTraceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPBMeleeHitBatchedSignature, const TArray<FHitResult>&, HitResults);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class LYRAGAME_API UPBMeleeWeaponTraceComponent : public URamaMeleeWeapon
{
	GENERATED_BODY()


public:
	UPBMeleeWeaponTraceComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	/**Like base Rama event except batches all hits from tick into one event */
	UPROPERTY(BlueprintAssignable, Category = "Rama Melee Weapon|PB")
	FPBMeleeHitBatchedSignature PBMeleeWeapon_OnHitBatched;

	UPROPERTY(EditDefaultsOnly, Category = "Rama Melee Weapon|PB")
	TEnumAsByte<ECollisionChannel> TraceChannel;

	virtual bool MeleeSweep(FHitResult& Hit, const TArray<FTransform>& BodyPreviousPose) override;
};
