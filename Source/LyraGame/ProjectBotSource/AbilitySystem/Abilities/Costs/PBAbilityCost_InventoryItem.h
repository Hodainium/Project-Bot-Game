
#pragma once

#include "ScalableFloat.h"
#include "Templates/SubclassOf.h"


#include "AbilitySystem/Abilities/LyraAbilityCost.h"
#include "PBAbilityCost_InventoryItem.generated.h"

class UPBItemDefinition;
struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpecHandle;

class ULyraGameplayAbility;
class UObject;
struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;

/**
 * Represents a cost that requires expending a quantity of an inventory item
 */
UCLASS(meta=(DisplayName="PB Inventory Item"))
class UPBAbilityCost_InventoryItem : public ULyraAbilityCost
{
	GENERATED_BODY()

public:
	UPBAbilityCost_InventoryItem();

	//~ULyraAbilityCost interface
	virtual bool CheckCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of ULyraAbilityCost interface

protected:
	/** How much of the item to spend (keyed on ability level) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	FScalableFloat Quantity;

	/** Which item to consume */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	UPBItemDefinition* ItemDefinition;

	/** Which tag to send back as a response if this cost cannot be applied */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Costs)
	FGameplayTag FailureTag;
};
