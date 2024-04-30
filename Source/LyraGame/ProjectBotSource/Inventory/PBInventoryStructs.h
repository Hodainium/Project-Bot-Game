// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"
#include "Logging/StructuredLog.h"
#include "PBInventoryStructs.generated.h"

class UPBEquipmentInstance;
class UPBInventoryItemInstance;

USTRUCT(BlueprintType)
struct FPBInventorySlotStruct
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<UPBInventoryItemInstance>> SlotArray;

	UPROPERTY()
	int32 NumSlots = 0;

	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	UPROPERTY(NotReplicated)
	TObjectPtr<UPBEquipmentInstance> EquippedItem;
};

