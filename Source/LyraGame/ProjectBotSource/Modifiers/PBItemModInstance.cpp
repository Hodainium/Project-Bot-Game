// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Modifiers/PBItemModInstance.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"
#include "ProjectBotSource/Inventory/PBInventoryEnums.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"
#include "ProjectBotSource/Modifiers/PBItemModDefinition.h"

UPBItemModInstance::UPBItemModInstance() 
{
	ModQuality = EPBItemQuality::Quality0;
	LevelOffset = 0;
}

void UPBItemModInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ModDef);
	DOREPLIFETIME(ThisClass, LevelOffset);
	DOREPLIFETIME(ThisClass, ModQuality);
}

UPBItemModDefinition* UPBItemModInstance::GetModDefinition() const
{
	return ModDef;
}

EPBItemQuality UPBItemModInstance::GetModQuality() const
{
	return ModQuality;
}

float UPBItemModInstance::GetDisplayMagnitude() const
{
	if(ModDef)
	{
		return ModDef->GetDisplayMagnitude(GetModLevel());
	}
	return -2;
}

FText UPBItemModInstance::GetDisplayDescription() const
{
	if (ModDef)
	{
		return ModDef->GetModDescription(GetModLevel());
	}

	return FText::GetEmpty();
}

float UPBItemModInstance::GetModLevel() const
{
	return static_cast<int>(ModQuality) + 1 + LevelOffset;
}

void UPBItemModInstance::OnWeaponEquipped(UPBModifiedWeaponInstance* EquipmentInstance,
                                            FPBItemModDef_GrantedHandles* OutGrantedHandles)
{
	if (!EquipmentInstance || !ModDef)
	{
		UE_LOGFMT(LogPBGame, Error, "Attempted to add mods to item instance but equipment or moddef were null");
		return;
	}

	ModDef->ApplyToEquipmentInstance(EquipmentInstance, OutGrantedHandles, GetModLevel());
}

void UPBItemModInstance::SetModLevelOffset(float inLevelOffset)
{
	LevelOffset = inLevelOffset;
}

void UPBItemModInstance::SetModDefinition(UPBItemModDefinition* InDef)
{
	ModDef = InDef;
}

void UPBItemModInstance::SetModQuality(EPBItemQuality InQuality)
{
	ModQuality = InQuality;
}

//void UPBItemModInstance::RemoveFromWeaponInstance(UPBModifiedWeaponInstance* EquipmentInstance,
//	FPBItemModDef_GrantedHandles* GrantedHandles)
//{
//	if (!EquipmentInstance || !ModDef)
//	{
//		UE_LOGFMT(LogPBGame, Error, "Attempted to remove mods to item instance but equipment or moddef were null");
//		return;
//	}
//
//	if(GrantedHandles)
//	{
//		GrantedHandles->RemoveModFromEquipmentInstance(EquipmentInstance);
//	}
//}
