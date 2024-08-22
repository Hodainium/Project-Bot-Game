// Copyright Epic Games, Inc. All Rights Reserved.

#include "PBAbilityCost_InventoryItem.h"
#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "Character/LyraCharacter.h"
#include "Logging/StructuredLog.h"
#include "ProjectBotSource/Inventory/PBInventoryComponent.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PBAbilityCost_InventoryItem)

UE_DEFINE_GAMEPLAY_TAG(TAG_ABILITY_FAIL_COST2, "Ability.ActivateFail.Cost");

UPBAbilityCost_InventoryItem::UPBAbilityCost_InventoryItem(): ItemDefinition(nullptr)
{
	Quantity.SetValue(1.0f);
	FailureTag = TAG_ABILITY_FAIL_COST2;
}

bool UPBAbilityCost_InventoryItem::CheckCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ALyraCharacter* Char = Ability->GetLyraCharacterFromActorInfo())
	{
		if (UPBInventoryComponent* InventoryComponent = Char->GetComponentByClass<UPBInventoryComponent>())
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumItemsToConsumeReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumItemsToConsume = FMath::TruncToInt(NumItemsToConsumeReal);
			const bool bCanApplyCost = InventoryComponent->GetTotalItemCountByDefinition(ItemDefinition) >= NumItemsToConsume;

			// Inform other abilities why this cost cannot be applied
			if (!bCanApplyCost && OptionalRelevantTags && FailureTag.IsValid())
			{
				OptionalRelevantTags->AddTag(FailureTag);
			}
			return bCanApplyCost;
		}
	}

	return false;
}

void UPBAbilityCost_InventoryItem::ApplyCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		if (ALyraCharacter* Char = Ability->GetLyraCharacterFromActorInfo())
		{
			if (UPBInventoryComponent* InventoryComponent = Char->GetComponentByClass<UPBInventoryComponent>())
			{
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

				const float NumItemsToConsumeReal = Quantity.GetValueAtLevel(AbilityLevel);
				const int32 NumItemsToConsume = FMath::TruncToInt(NumItemsToConsumeReal);

				InventoryComponent->ConsumeItemsByDefinition(ItemDefinition, NumItemsToConsume);
			}
		}
	}
}

