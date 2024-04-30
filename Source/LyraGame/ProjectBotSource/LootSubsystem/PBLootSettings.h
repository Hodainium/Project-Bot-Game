// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DeveloperSettings.h"
#include "PBLootSettings.generated.h"

class UDataTable;

/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName = "PB|LootSettings"))
class LYRAGAME_API UPBLootSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Display|Text")
	TSoftObjectPtr<UDataTable> AdjectiveTable;
};
