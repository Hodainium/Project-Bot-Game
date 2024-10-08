// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Weapons/PBWeaponBase.h"

APBWeaponBase::APBWeaponBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APBWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APBWeaponBase::OnFireEffects_Implementation(const TArray<FVector>& ImpactPositions, const TArray<FVector>& ImpactNormals, EPhysicalSurface ImpactSurfaceTypes)
{
	UE_LOG(LogTemp, Error, TEXT("We should NOT be calling the base OnFire method on weaponbase"));
}

