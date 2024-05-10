// Fill out your copyright notice in the Description page of Project Settings.


#include "PBWeaponAttributeSet.h"

#include "Net/UnrealNetwork.h"

void UPBWeaponAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBWeaponAttributeSet, WeaponDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBWeaponAttributeSet, WeaponFireRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBWeaponAttributeSet, WeaponRange, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBWeaponAttributeSet, WeaponAccuracy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBWeaponAttributeSet, WeaponReloadSpeed, COND_None, REPNOTIFY_Always);
}

UPBWeaponAttributeSet::UPBWeaponAttributeSet()
	: WeaponDamage(0.f)
	, WeaponFireRate(0.f)
	, WeaponRange(0.f)
	, WeaponAccuracy(0.f)
	, WeaponReloadSpeed(0.f)
{
}

void UPBWeaponAttributeSet::OnRep_WeaponDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponDamage, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	OnWeaponDamageChanged.Broadcast(nullptr, nullptr, nullptr, GetWeaponDamage() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetWeaponDamage());
}

void UPBWeaponAttributeSet::OnRep_WeaponFireRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponFireRate, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	OnWeaponFireRateChanged.Broadcast(nullptr, nullptr, nullptr, GetWeaponFireRate() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetWeaponFireRate());
}

void UPBWeaponAttributeSet::OnRep_WeaponRange(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponRange, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	OnWeaponRangeChanged.Broadcast(nullptr, nullptr, nullptr, GetWeaponRange() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetWeaponRange());
}

void UPBWeaponAttributeSet::OnRep_WeaponAccuracy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponAccuracy, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	OnWeaponAccuracyChanged.Broadcast(nullptr, nullptr, nullptr, GetWeaponAccuracy() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetWeaponAccuracy());
}

void UPBWeaponAttributeSet::OnRep_WeaponReloadSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponReloadSpeed, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	OnWeaponReloadSpeedChanged.Broadcast(nullptr, nullptr, nullptr, GetWeaponReloadSpeed() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetWeaponReloadSpeed());
}
