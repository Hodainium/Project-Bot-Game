// Fill out your copyright notice in the Description page of Project Settings.



#include "PBGameplayAbility_FromEquipment.h"
#include "PBEquipmentInstance.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include "ProjectBotSource/Inventory/PBItemInstance.h"

UPBGameplayAbility_FromEquipment::UPBGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UPBEquipmentInstance* UPBGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UPBEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}

UPBInventoryItemInstance* UPBGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UPBEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UPBInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}


#if WITH_EDITOR
EDataValidationResult UPBGameplayAbility_FromEquipment::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		Context.AddError(NSLOCTEXT("ProjBot", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}

#endif
