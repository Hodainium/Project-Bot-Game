// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Weapons/PBModifiedWeaponInstance.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"
#include "ProjectBotSource/Inventory/PBItemInstance.h"
#include "Logging/StructuredLog.h"
#include "ProjectBotSource/Modifiers/PBItemModInstance.h"

void UPBModifiedWeaponInstance::OnEquipped()
{
	Super::OnEquipped();

	if(!(GetPawn() && GetPawn()->HasAuthority()))
	{
		return;
	}

	if(UPBInventoryItemInstance* ItemInstance = Cast<UPBInventoryItemInstance>(GetInstigator()))
	{
		//Untested but here we will grab mods from item instance and apply

		for (UPBItemModInstance* Mod : ItemInstance->GetItemMods())
		{
			if (Mod != nullptr)
			{
				FPBItemModDef_GrantedHandles GrantedHandles;
				Mod->OnWeaponEquipped(this, &GrantedHandles);
				AppliedModHandles.Add(GrantedHandles);
			}
		}
	}
}

void UPBModifiedWeaponInstance::OnUnequipped()
{
	Super::OnUnequipped();

	//For each mod in array remove mod

	if (!(GetPawn() && GetPawn()->HasAuthority()))
	{
		return;
	}

	if (UPBInventoryItemInstance* ItemInstance = Cast<UPBInventoryItemInstance>(GetInstigator()))
	{
		//Untested but here we will grab mods from item instance and apply

		for (FPBItemModDef_GrantedHandles& Handle : AppliedModHandles)
		{
			Handle.RemoveModFromEquipmentInstance(this);
		}

		AppliedModHandles.Reset();
	}
}

void UPBModifiedWeaponInstance::AddDamageGE(TSubclassOf<UGameplayEffect> Effect)
{
	DamageGEArray.Add(Effect);
}

void UPBModifiedWeaponInstance::RemoveDamageGE(TSubclassOf<UGameplayEffect> Effect)
{
	DamageGEArray.RemoveSingle(Effect);
}

void UPBModifiedWeaponInstance::AddEffectOnHit(TSubclassOf<UGameplayEffect> Effect)
{
	AdditionalEffectsToApplyOnHit.Add(Effect);
}

void UPBModifiedWeaponInstance::RemoveEffectOnHit(TSubclassOf<UGameplayEffect> Effect)
{
	AdditionalEffectsToApplyOnHit.RemoveSingle(Effect);
}
