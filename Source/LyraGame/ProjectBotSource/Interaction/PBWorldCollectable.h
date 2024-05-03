// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "IPBInteractableTarget.h"
#include "PBInteractionOption.h"
#include "IPBPickupable.h"
#include "PBWorldInteractable.h"
#include "PBWorldCollectable.generated.h"

class UObject;
struct FPBInteractionQuery;

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class APBWorldCollectable : public APBWorldInteractable, public IPBPickupable
{
	GENERATED_BODY()

public:

	APBWorldCollectable();

	virtual FPBInventoryPickup GetPickupInventory() const override;

protected:

	UPROPERTY(EditAnywhere)
	FPBInventoryPickup StaticInventory;
};

UCLASS(Abstract, Blueprintable)
class APBWorldCollectableInstance : public APBWorldInteractable, public IPBPickupable
{
	GENERATED_BODY()

public:

	APBWorldCollectableInstance();

	virtual FPBInventoryPickup GetPickupInventory() const override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnItemInstanceSet();

	UFUNCTION()
	void OnRep_ItemInstance();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = "OnRep_ItemInstance", meta = (ExposeOnSpawn = true))
	TObjectPtr<UPBInventoryItemInstance> ItemInstance = nullptr;
};
