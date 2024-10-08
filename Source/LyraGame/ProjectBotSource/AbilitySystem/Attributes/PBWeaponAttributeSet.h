// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "PBWeaponAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API UPBWeaponAttributeSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:

	UPBWeaponAttributeSet();

	ATTRIBUTE_ACCESSORS(UPBWeaponAttributeSet, WeaponDamageMultiplier);
	ATTRIBUTE_ACCESSORS(UPBWeaponAttributeSet, WeaponFireRateMultiplier);
	ATTRIBUTE_ACCESSORS(UPBWeaponAttributeSet, WeaponRangeMultiplier);
	ATTRIBUTE_ACCESSORS(UPBWeaponAttributeSet, WeaponAccuracyMultiplier);
	ATTRIBUTE_ACCESSORS(UPBWeaponAttributeSet, WeaponReloadSpeedMultiplier);
	ATTRIBUTE_ACCESSORS(UPBWeaponAttributeSet, ItemUseSpeedMultiplier);
	ATTRIBUTE_ACCESSORS(UPBWeaponAttributeSet, WeaponSwingSpeedMultiplier);

protected:

	UFUNCTION()
	void OnRep_WeaponDamageMultiplier(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_WeaponFireRateMultiplier(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_WeaponRangeMultiplier(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_WeaponAccuracyMultiplier(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_WeaponReloadSpeedMultiplier(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_ItemUseSpeedMultiplier(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_WeaponSwingSpeedMultiplier(const FGameplayAttributeData& OldValue);


private:

	// The current damage attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponDamageMultiplier, Category = "PB|Weapon", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponDamageMultiplier;

	// The current firerate attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponFireRateMultiplier, Category = "PB|Weapon", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponFireRateMultiplier;

	// The current range attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponRangeMultiplier, Category = "PB|Weapon", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponRangeMultiplier;

	// The current accuracy attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponAccuracyMultiplier, Category = "PB|Weapon", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponAccuracyMultiplier;

	// The current accuracy attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponReloadSpeedMultiplier, Category = "PB|Weapon", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponReloadSpeedMultiplier;

	// The current accuracy attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ItemUseSpeedMultiplier, Category = "PB|Weapon", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ItemUseSpeedMultiplier;

	// The current accuracy attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponSwingSpeedMultiplier, Category = "PB|Weapon", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponSwingSpeedMultiplier;
};