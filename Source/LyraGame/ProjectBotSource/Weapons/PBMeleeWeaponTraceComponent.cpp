// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Weapons/PBMeleeWeaponTraceComponent.h"


UPBMeleeWeaponTraceComponent::UPBMeleeWeaponTraceComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPBMeleeWeaponTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	RamaMeleeWeapon_OnHit.AddDynamic(this, &UPBMeleeWeaponTraceComponent::HandleRamaEvent);
}

bool UPBMeleeWeaponTraceComponent::MeleeSweep(FHitResult& Hit, const TArray<FTransform>& BodyPreviousPose)
{
	if(!BatchedHitResults.IsEmpty())
	{
		BatchedHitResults.Reset();
	}

	bool didHit = Super::MeleeSweep(Hit, BodyPreviousPose);

	if(PBMeleeWeapon_OnHitBatched.IsBound())
	{
		if(BatchedHitResults.Num() > 0)
		{
			PBMeleeWeapon_OnHitBatched.Broadcast(BatchedHitResults);
		}
	}

	BatchedHitResults.Reset();

	return didHit;
}

void UPBMeleeWeaponTraceComponent::HandleRamaEvent(AActor* HitActor, UPrimitiveComponent* HitComponent,
	const FVector& ImpactPoint, const FVector& ImpactNormal, int32 ShapeIndex, FName HitBoneName,
	const FHitResult& HitResult)
{
	BatchedHitResults.Add(HitResult);
}

