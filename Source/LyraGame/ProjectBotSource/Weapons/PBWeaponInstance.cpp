// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Weapons/PBWeaponInstance.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"

UPBWeaponInstance::UPBWeaponInstance(const FObjectInitializer& ObjectInitializer)
{
}

void UPBWeaponInstance::OnEquipped()
{
	Super::OnEquipped();

	UWorld* World = GetWorld();
	check(World);
	TimeLastEquipped = World->GetTimeSeconds();
}

void UPBWeaponInstance::OnUnequipped()
{
	Super::OnUnequipped();
}

void UPBWeaponInstance::UpdateFiringTime()
{
	UWorld* World = GetWorld();
	check(World); 
	TimeLastEquipped = World->GetTimeSeconds();
}

float UPBWeaponInstance::GetTimeSinceLastInteractedWith() const
{
	UWorld* World = GetWorld();
	check(World);
	const double WorldTime = World->GetTimeSeconds();

	double Result = WorldTime - TimeLastEquipped;

	if (TimeLastFired > 0.0)
	{
		const double TimeSinceFired = WorldTime - TimeLastFired;
		Result = FMath::Min(Result, TimeSinceFired);
	}

	return Result;
}

TSubclassOf<UAnimInstance> UPBWeaponInstance::PickBestAnimLayer(bool bEquipped,
	const FGameplayTagContainer& CosmeticTags) const
{
	const FLyraAnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UnequippedAnimSet);
	return SetToQuery.SelectBestLayer(CosmeticTags);
}

ULyraAbilitySystemComponent* UPBWeaponInstance::GetASCFromOwningPawn()
{
	if(APawn* Pawn = GetPawn())
	{
		return Cast<ULyraAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn));
	}

	return nullptr;
}
