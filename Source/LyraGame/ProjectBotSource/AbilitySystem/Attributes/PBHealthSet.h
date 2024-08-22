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
	ATTRIBUTE_ACCESSORS(UPBHealthSet, Knockback);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

protected:

	UFUNCTION()
	void OnRep_HealthRegenRate(const FGameplayAttributeData& OldValue);

	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const override;

private:
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegenRate, Category = "PB|Attributes|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HealthRegenRate;

	// Incoming knockback. To be cleared after using. SHould eventually be hidden from modifiers but for now it's ok
	UPROPERTY(BlueprintReadOnly, Category = "PB|Health", Meta = (AllowPrivateAccess = true)) //HideFromModifiers, 
	FGameplayAttributeData Knockback;
};
