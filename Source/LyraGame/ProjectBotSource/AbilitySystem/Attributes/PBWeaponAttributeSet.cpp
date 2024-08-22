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
	DOREPLIFETIME_CONDITION_NOTIFY(UPBWeaponAttributeSet, WeaponSwingSpeedMultiplier, COND_None, REPNOTIFY_Always);
}

UPBWeaponAttributeSet::UPBWeaponAttributeSet()
	: WeaponDamageMultiplier(1.f)
	, WeaponFireRateMultiplier(1.f)
	, WeaponRangeMultiplier(1.f)
	, WeaponAccuracyMultiplier(1.f)
	, WeaponReloadSpeedMultiplier(1.f)
	, ItemUseSpeedMultiplier(1.f)
	, WeaponSwingSpeedMultiplier(1.f)
{
}

void UPBWeaponAttributeSet::OnRep_WeaponDamageMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponDamageMultiplier, OldValue);
}

void UPBWeaponAttributeSet::OnRep_WeaponFireRateMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponFireRateMultiplier, OldValue);
}

void UPBWeaponAttributeSet::OnRep_WeaponRangeMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponRangeMultiplier, OldValue);
}

void UPBWeaponAttributeSet::OnRep_WeaponAccuracyMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponAccuracyMultiplier, OldValue);
}

void UPBWeaponAttributeSet::OnRep_WeaponReloadSpeedMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponReloadSpeedMultiplier, OldValue);
}

void UPBWeaponAttributeSet::OnRep_ItemUseSpeedMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, ItemUseSpeedMultiplier, OldValue);
}

void UPBWeaponAttributeSet::OnRep_WeaponSwingSpeedMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBWeaponAttributeSet, WeaponSwingSpeedMultiplier, OldValue);
}
