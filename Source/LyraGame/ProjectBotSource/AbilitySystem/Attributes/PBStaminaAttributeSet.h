// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "PBStaminaAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API UPBStaminaAttributeSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:

	UPBStaminaAttributeSet();

	ATTRIBUTE_ACCESSORS(UPBStaminaAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UPBStaminaAttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UPBStaminaAttributeSet, StaminaRegenRate)

	// Delegate when stamina changes, some information may be missing on the client
	mutable FLyraAttributeEvent OnStaminaChanged;

	// Delegate when maxStamina changes
	mutable FLyraAttributeEvent OnMaxStaminaChanged;

protected:

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_StaminaRegenRate(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;


private:

	// Store the Stamina before any changes 
	float MaxStaminaBeforeAttributeChange;
	float StaminaBeforeAttributeChange;

	// The current damage attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "PB|Attributes|Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	// The current firerate attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "PB|Attributes|Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaRegenRate, Category = "PB|Attributes|Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StaminaRegenRate;
};