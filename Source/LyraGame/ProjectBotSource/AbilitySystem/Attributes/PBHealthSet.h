// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "PBHealthSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class LYRAGAME_API UPBHealthSet : public ULyraHealthSet
{
	GENERATED_BODY()

public:

	UPBHealthSet();

	ATTRIBUTE_ACCESSORS(UPBHealthSet, HealthRegenRate);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;

protected:

	UFUNCTION()
	void OnRep_HealthRegenRate(const FGameplayAttributeData& OldValue);

	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const override;

private:
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegenRate, Category = "PB|Attributes|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HealthRegenRate;
};
