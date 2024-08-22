// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/AbilitySystem/AdditionalCosts/PBAbilityCost_ConditonalBlockTag.h"

#include "AbilitySystemComponent.h"


UPBAbilityCost_ConditonalBlockTag::UPBAbilityCost_ConditonalBlockTag()
{
}

bool UPBAbilityCost_ConditonalBlockTag::CheckCost(const ULyraGameplayAbility* Ability,
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();

	//Only check override tag if we have blocking tag. otherwise return true
	if(AbilitySystemComponent->HasMatchingGameplayTag(BlockingTag))
	{
		//If we have the override tag return true
		return AbilitySystemComponent->HasMatchingGameplayTag(OverrideTag);
	}

	return true;
}
