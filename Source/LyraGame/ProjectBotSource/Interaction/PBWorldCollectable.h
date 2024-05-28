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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual FPBInventoryPickup GetPickupInventory() const override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void K2_OnItemInstanceSet();

	UFUNCTION(BlueprintCallable)
	void SetItemInstance(UPBInventoryItemInstance* InItem);

	

	UFUNCTION()
	void OnRep_ItemInstance();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnConstruction(const FTransform& Transform) override;

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface



protected:

	void OnItemInstanceSet();

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = "OnRep_ItemInstance", meta = (ExposeOnSpawn = true))
	TObjectPtr<UPBInventoryItemInstance> ItemInstance = nullptr;
};
