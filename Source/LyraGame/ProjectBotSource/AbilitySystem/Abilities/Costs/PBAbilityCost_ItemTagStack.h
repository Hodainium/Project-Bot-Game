// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

#include "ScalableFloat.h"
#include "AbilitySystem/Abilities/LyraAbilityCost.h"

#include "PBAbilityCost_ItemTagStack.generated.h"

struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpecHandle;

class ULyraGameplayAbility;
class UObject;
struct FGameplayAbilityActorInfo;

/**
 * Represents a cost that requires expending a quantity of a tag stack
 * on the associated item instance
 */
UCLASS(meta=(DisplayName="PB Item Tag Stack"))
class UPBAbilityCost_ItemTagStack : public ULyraAbilityCost
{
	GENERATED_BODY()

public:
	UPBAbilityCost_ItemTagStack();

	//~ULyraAbilityCost interface
	virtual bool CheckCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of ULyraAbilityCost interface

protected:
	/** How much of the tag to spend (keyed on ability level) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FScalableFloat Quantity;

	/** Which tag to spend some of */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FGameplayTag Tag;

	/** Which tag to send back as a response if this cost cannot be applied */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FGameplayTag FailureTag;
};
