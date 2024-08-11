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

	ATTRIBUTE_ACCESSORS(UPBStatsSet, Strength);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxStrength);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, PowerBank);
	ATTRIBUTE_ACCESSORS(UPBStatsSet, MaxPowerBank);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxStrength(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_PowerBank(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxPowerBank(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Strength;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStrength, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStrength;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PowerBank, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PowerBank;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxPowerBank, Category = "PB|Stats", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxPowerBank;
};
