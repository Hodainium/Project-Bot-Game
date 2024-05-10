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

	ATTRIBUTE_ACCESSORS(UPBWeaponAttributeSet, WeaponDamage);
	ATTRIBUTE_ACCESSORS(UPBWeaponAttributeSet, WeaponFireRate);
	ATTRIBUTE_ACCESSORS(UPBWeaponAttributeSet, WeaponRange);
	ATTRIBUTE_ACCESSORS(UPBWeaponAttributeSet, WeaponAccuracy);
	ATTRIBUTE_ACCESSORS(UPBWeaponAttributeSet, WeaponReloadSpeed);

	//ATTRIBUTE_ACCESSORS(UHWeaponAttributeSet, Element);

	// Delegate when WeaponDamage changes, some information may be missing on the client
	mutable FLyraAttributeEvent OnWeaponDamageChanged;

	// Delegate when FireRate changes
	mutable FLyraAttributeEvent OnWeaponFireRateChanged;

	// Delegate when range changes
	mutable FLyraAttributeEvent OnWeaponRangeChanged;

	// Delegate when accuracy changes, some information may be missing on the client
	mutable FLyraAttributeEvent OnWeaponAccuracyChanged;

	// Delegate when accuracy changes, some information may be missing on the client
	mutable FLyraAttributeEvent OnWeaponReloadSpeedChanged;

protected:

	UFUNCTION()
	void OnRep_WeaponDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_WeaponFireRate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_WeaponRange(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_WeaponAccuracy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_WeaponReloadSpeed(const FGameplayAttributeData& OldValue);


private:

	// The current damage attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponDamage, Category = "PB|Weapon", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponDamage;

	// The current firerate attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponFireRate, Category = "PB|Weapon", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponFireRate;

	// The current range attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponRange, Category = "PB|Weapon", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponRange;

	// The current accuracy attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponAccuracy, Category = "PB|Weapon", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponAccuracy;

	// The current accuracy attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponReloadSpeed, Category = "PB|Weapon", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WeaponReloadSpeed;
};