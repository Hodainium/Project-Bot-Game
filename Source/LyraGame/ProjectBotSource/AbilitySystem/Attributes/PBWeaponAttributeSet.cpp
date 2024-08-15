// Fill out your copyright notice in the Description page of Project Settings.


#include "PBWeaponAttributeSet.h"

#include "Net/UnrealNetwork.h"

void UPBWeaponAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBWeaponAttributeSet, WeaponDamageMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBWeaponAttributeSet, WeaponFireRateMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBWeaponAttributeSet, WeaponRangeMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBWeaponAttributeSet, WeaponAccuracyMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBWeaponAttributeSet, WeaponReloadSpeedMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBWeaponAttributeSet, ItemUseSpeedMultiplier, COND_None, REPNOTIFY_Always);
}

UPBWeaponAttributeSet::UPBWeaponAttributeSet()
	: WeaponDamageMultiplier(1.f)
	, WeaponFireRateMultiplier(1.f)
	, WeaponRangeMultiplier(1.f)
	, WeaponAccuracyMultiplier(1.f)
	, WeaponReloadSpeedMultiplier(1.f)
	, ItemUseSpeedMultiplier(1.f)
{
}

void UPBWeaponAttributeSet::OnRep_WeaponDamageMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponDamageMultiplier, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	OnWeaponDamageChanged.Broadcast(nullptr, nullptr, nullptr, GetWeaponDamageMultiplier() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetWeaponDamageMultiplier());
}

void UPBWeaponAttributeSet::OnRep_WeaponFireRateMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponFireRateMultiplier, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	OnWeaponFireRateChanged.Broadcast(nullptr, nullptr, nullptr, GetWeaponFireRateMultiplier() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetWeaponFireRateMultiplier());
}

void UPBWeaponAttributeSet::OnRep_WeaponRangeMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponRangeMultiplier, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	OnWeaponRangeChanged.Broadcast(nullptr, nullptr, nullptr, GetWeaponRangeMultiplier() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetWeaponRangeMultiplier());
}

void UPBWeaponAttributeSet::OnRep_WeaponAccuracyMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponAccuracyMultiplier, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	OnWeaponAccuracyChanged.Broadcast(nullptr, nullptr, nullptr, GetWeaponAccuracyMultiplier() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetWeaponAccuracyMultiplier());
}

void UPBWeaponAttributeSet::OnRep_WeaponReloadSpeedMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponReloadSpeedMultiplier, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	OnWeaponReloadSpeedChanged.Broadcast(nullptr, nullptr, nullptr, GetWeaponReloadSpeedMultiplier() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetWeaponReloadSpeedMultiplier());
}

void UPBWeaponAttributeSet::OnRep_ItemUseSpeedMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, ItemUseSpeedMultiplier, OldValue);

	ItemUseSpeedMultiplierChanged.Broadcast(nullptr, nullptr, nullptr, GetWeaponReloadSpeedMultiplier() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetWeaponReloadSpeedMultiplier());
}
