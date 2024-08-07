// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "NativeGameplayTags.h"
#include "PBStatComponent.generated.h"

class UPBItemDefinition;
class UPBInventoryComponent;
struct FPBInventoryList;
class UPBInventoryItemInstance;

//UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_Item_Count, "ItemStatTags.Inventory.UseItem.Count");

UCLASS(BlueprintType)
class LYRAGAME_API UPBStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPBStatComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:

};
