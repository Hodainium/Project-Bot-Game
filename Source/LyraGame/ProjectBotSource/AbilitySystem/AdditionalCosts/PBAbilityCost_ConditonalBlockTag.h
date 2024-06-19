// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LyraAbilityCost.h"
#include "PBAbilityCost_ConditonalBlockTag.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Conditional Blocking Tag"))
class LYRAGAME_API UPBAbilityCost_ConditonalBlockTag : public ULyraAbilityCost
{
	GENERATED_BODY()

public:

	UPBAbilityCost_ConditonalBlockTag();

	virtual bool CheckCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	
protected:
	/** Which tag blocks */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FGameplayTag BlockingTag;

	/** Which tag overrides the block */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FGameplayTag OverrideTag;
};
