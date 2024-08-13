// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "PBStatsSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class LYRAGAME_API UPBStatsSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:

	UPBStatsSet();

	ATTRIBUTE_ACCESSORS(UPBStatsSet, PowerBank);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxPowerBank);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Strength);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxStrength);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Dexterity);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxDexterity);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Marksmanship);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxMarksmanship);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Radiance);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxRadiance);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Strike);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxStrike);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Pierce);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxPierce);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Slash);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxSlash);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Fire);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxFire);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Ice);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxIce);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Lightning);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxLightning);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Decay);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxDecay);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Armor);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxArmor);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Energy);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxEnergy);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Repair);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxRepair);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Scan);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxScan);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Shield);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxShield);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Grab);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxGrab);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Charge);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxCharge);

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Special);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxSpecial);
	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UFUNCTION()
	void OnRep_PowerBank(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxPowerBank(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxStrength(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxDexterity(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Marksmanship(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxMarksmanship(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Radiance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxRadiance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Strike(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxStrike(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Pierce(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxPierce(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Slash(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxSlash(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Fire(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxFire(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Ice(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxIce(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Lightning(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxLightning(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Decay(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxDecay(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxArmor(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Energy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxEnergy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Repair(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxRepair(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Scan(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxScan(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Shield(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxShield(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Grab(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxGrab(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Charge(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxCharge(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Special(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxSpecial(const FGameplayAttributeData& OldValue);

	

private:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PowerBank, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PowerBank;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxPowerBank, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxPowerBank;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Strength;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStrength, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStrength;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dexterity, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Dexterity;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxDexterity, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxDexterity;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Marksmanship, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Marksmanship;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMarksmanship, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxMarksmanship;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Radiance, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Radiance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxRadiance, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxRadiance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strike, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Strike;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStrike, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStrike;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Pierce, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Pierce;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxPierce, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxPierce;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Slash, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Slash;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxSlash, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSlash;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Fire, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Fire;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxFire, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxFire;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Ice, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Ice;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxIce, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxIce;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Lightning, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Lightning;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxLightning, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxLightning;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Decay, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Decay;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxDecay, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxDecay;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Armor;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxArmor, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxArmor;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Energy, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Energy;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxEnergy, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxEnergy;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Repair, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Repair;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxRepair, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxRepair;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Scan, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Scan;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxScan, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxScan;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Shield;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxShield, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxShield;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Grab, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Grab;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxGrab, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxGrab;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Charge, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Charge;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxCharge, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxCharge;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Special, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Special;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxSpecial, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSpecial;

};
