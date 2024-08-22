// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "NativeGameplayTags.h"
#include "PBInventoryComponent.generated.h"

class UPBItemDefinition;
class UPBInventoryComponent;
struct FPBInventoryList;
class UPBInventoryItemInstance;

//UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_Item_Count, "ItemStatTags.Inventory.UseItem.Count");

/** A message when an item is added to the inventory */
USTRUCT(BlueprintType)
struct FPBInventoryItemCountChangedMessage
{
	GENERATED_BODY()

	//@TODO: Tag based names+owning actors for inventories instead of directly exposing the component?
	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UPBInventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	int32 Delta = 0;
};

USTRUCT(BlueprintType)
struct FPBInventoryChangedMessage
{
	GENERATED_BODY()

	//@TODO: Tag based names+owning actors for inventories instead of directly exposing the component?
	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UPBItemDefinition> ItemDef = nullptr;
};

USTRUCT(BlueprintType)
struct FPBInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FPBInventoryEntry(): Index(-1)
	{
	}

	FPBInventoryEntry(UPBInventoryItemInstance* InInstance, int32 InStackCount) : Instance(InInstance),
		Index(-1),
		StackCount(InStackCount)
	{
	}

	FString GetDebugString() const;

private:
	friend FPBInventoryList;
	friend UPBInventoryComponent;

public:
	UPROPERTY()
	TObjectPtr<UPBInventoryItemInstance> Instance = nullptr;

	UPROPERTY()
	uint8 Index;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

/** List of inventory items */
USTRUCT(BlueprintType)
struct FPBInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FPBInventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FPBInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	TArray<UPBInventoryItemInstance*> GetAllItems() const;

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	void PostReplicatedReceive(const FPostReplicatedReceiveParameters& Parameters);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FPBInventoryEntry, FPBInventoryList>(Entries, DeltaParms, *this);
	}

	UPBInventoryItemInstance* AddEntry(UPBItemDefinition* ItemDef, int32 StackCount);

	UPBInventoryItemInstance* AddEntry(UPBInventoryItemInstance* Instance);

	void ChangeEntryStackCount(FPBInventoryEntry* InEntry, int StackCount);

	void RemoveEntry(UPBInventoryItemInstance* Instance);

	bool MarkItemIDDirty(int32 ItemID);
	

private:
	friend UPBInventoryComponent;
	//friend UPBGridInventoryComponent;

	void BroadcastChangeMessage(FPBInventoryEntry& Entry, int32 OldCount, int32 NewCount);
	void BroadcastInventoryItemCountChangedMessage(UPBItemDefinition* InItemDef);


private:
	// Replicated list of items
	UPROPERTY()
	TArray<FPBInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FPBInventoryList> : public TStructOpsTypeTraitsBase2<FPBInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

UCLASS(BlueprintType)
class LYRAGAME_API UPBInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPBInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	bool CanAddItemDefinition(UPBItemDefinition* ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	bool CanStackItemDefinition(UPBItemDefinition* ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	UPBInventoryItemInstance* AddItemDefinition(UPBItemDefinition* ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	UPBInventoryItemInstance* AddItemInstance(UPBInventoryItemInstance* ItemInstance);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void RemoveItemInstance(UPBInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category = Inventory, BlueprintPure = false)
	TArray<UPBInventoryItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category = Inventory, BlueprintPure)
	UPBInventoryItemInstance* FindFirstItemInstanceByDefinition(UPBItemDefinition* ItemDef) const;

	FPBInventoryEntry* FindFirstItemStackByDefinition(UPBItemDefinition* ItemDef);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	int32 GetTotalItemCountByDefinition(UPBItemDefinition* ItemDef) const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool ConsumeItemsByDefinition(UPBItemDefinition* ItemDef, int32 NumToConsume);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UObject interface

private:
	UPROPERTY(Replicated)
	FPBInventoryList InventoryList;
};
