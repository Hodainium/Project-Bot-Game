// Copyright Epic Games, Inc. All Rights Reserved.

#include "PBAbilityCost_ItemTagStack.h"

#include "NativeGameplayTags.h"
#include "ProjectBotSource/Equipment/PBGameplayAbility_FromEquipment.h"
#include "ProjectBotSource/Inventory/PBItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PBAbilityCost_ItemTagStack)

UE_DEFINE_GAMEPLAY_TAG(TAG_ABILITY_FAIL_COST3, "Ability.ActivateFail.Cost");

UPBAbilityCost_ItemTagStack::UPBAbilityCost_ItemTagStack()
{
	Quantity.SetValue(1.0f);
	FailureTag = TAG_ABILITY_FAIL_COST3;
}

bool UPBAbilityCost_ItemTagStack::CheckCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (const UPBGameplayAbility_FromEquipment* EquipmentAbility = Cast<const UPBGameplayAbility_FromEquipment>(Ability))
	{
		if (UPBInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt(NumStacksReal);
			const bool bCanApplyCost = ItemInstance->GetStatTagStackCount(Tag) >= NumStacks;

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

void UPBAbilityCost_ItemTagStack::ApplyCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		if (const UPBGameplayAbility_FromEquipment* EquipmentAbility = Cast<const UPBGameplayAbility_FromEquipment>(Ability))
		{
			if (UPBInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
			{
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

				const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
				const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

				ItemInstance->RemoveStatTagStack(Tag, NumStacks);
			}
		}
	}
}