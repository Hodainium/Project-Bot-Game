// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Inventory/PBInventoryComponent.h"
#include "NativeGameplayTags.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "ProjectBotSource/Inventory/PBItemDefinition.h"
#include "ProjectBotSource/Inventory/PBItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "ProjectBotSource/Modifiers/PBItemModInstance.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Inventory_Message_StackChanged, "Inventory.Message.StackChanged");
//UE_DEFINE_GAMEPLAY_TAG(TAG_Inventory_Item_Count, "ItemStatTags.Inventory.Item.Count");

FString FPBInventoryEntry::GetDebugString() const
{
	UPBItemDefinition* ItemDef = nullptr;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDefinition();
	}

	return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));
}

TArray<UPBInventoryItemInstance*> FPBInventoryList::GetAllItems() const
{
	TArray<UPBInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FPBInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

void FPBInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FPBInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FPBInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FPBInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FPBInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FPBInventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FPBInventoryList::PostReplicatedReceive(const FPostReplicatedReceiveParameters& Parameters)
{

}

void FPBInventoryList::RemoveEntry(UPBInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPBInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

bool FPBInventoryList::MarkItemIDDirty(int32 ItemID)
{
	for (FPBInventoryEntry& Entry :Entries)
	{
		if(Entry.ReplicationID == ItemID)
		{
			MarkItemDirty(Entry);
			return true;
		}
	}
	return false;
}

void FPBInventoryList::BroadcastChangeMessage(FPBInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	FPBInventoryChangeMessage Message;
	Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;

	//Entry.Instance->AddStatTagStack(TAG_Inventory_Item_Count, NewCount);

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
	MessageSystem.BroadcastMessage(TAG_Inventory_Message_StackChanged, Message);
}

UPBInventoryItemInstance* FPBInventoryList::AddEntry(UPBItemDefinition* ItemDef, int32 StackCount)
{
	UPBInventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FPBInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UPBInventoryItemInstance>(OwnerComponent->GetOwner());  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDef);
	for (UPBInventoryItemFragment* Fragment : ItemDef->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;

	//NewEntry.Instance->AddStatTagStack(TAG_Inventory_Item_Count, StackCount);

	MarkItemDirty(NewEntry);

	return Result;
}

void FPBInventoryList::AddEntry(UPBInventoryItemInstance* Instance)
{
	check(Instance != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FPBInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = Instance;  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.StackCount = 1;

	MarkItemDirty(NewEntry);
}

UPBInventoryComponent::UPBInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

bool UPBInventoryComponent::CanAddItemDefinition(UPBItemDefinition* ItemDef, int32 StackCount)
{
	//We can do logic here for item limits and such

	return true;
}

bool UPBInventoryComponent::CanStackItemDefinition(UPBItemDefinition* ItemDef, int32 StackCount)
{
	/*if(ItemDef->CanBeStacked)
	{
		for (FPBInventoryEntry& Entry : InventoryArray.Entries)
		{
			InventoryGrid[0] = nullptr;
		}
	}*/

	return false;
}

UPBInventoryItemInstance* UPBInventoryComponent::AddItemDefinition(UPBItemDefinition* ItemDef, int32 StackCount)
{
	UPBInventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);

		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void UPBInventoryComponent::AddItemInstance(UPBInventoryItemInstance* ItemInstance)
{
	InventoryList.AddEntry(ItemInstance);
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
		for (UPBItemModInstance* Mod : ItemInstance->GetItemMods())
		{
			AddReplicatedSubObject(Mod);
		}
	}
}

void UPBInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UPBInventoryComponent::RemoveItemInstance(UPBInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		for (UPBItemModInstance* Mod : ItemInstance->GetItemMods())
		{
			RemoveReplicatedSubObject(Mod);
		}
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UPBInventoryItemInstance*> UPBInventoryComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

UPBInventoryItemInstance* UPBInventoryComponent::FindFirstItemStackByDefinition(
	UPBItemDefinition* ItemDef) const
{
	for (const FPBInventoryEntry& Entry : InventoryList.Entries)
	{
		UPBInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDefinition() == ItemDef)
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

int32 UPBInventoryComponent::GetTotalItemCountByDefinition(UPBItemDefinition* ItemDef) const
{
	int32 TotalCount = 0;
	for (const FPBInventoryEntry& Entry : InventoryList.Entries)
	{
		UPBInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDefinition() == ItemDef)
			{
				TotalCount++;
			}
		}
	}

	return TotalCount;
}

bool UPBInventoryComponent::ConsumeItemsByDefinition(UPBItemDefinition* ItemDef, int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	int32 TotalConsumed = 0;
	while (TotalConsumed < NumToConsume)
	{
		if (UPBInventoryItemInstance* Instance = UPBInventoryComponent::FindFirstItemStackByDefinition(ItemDef))
		{
			InventoryList.RemoveEntry(Instance);
			TotalConsumed++;
		}
		else
		{
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

bool UPBInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FPBInventoryEntry& Entry : InventoryList.Entries)
	{
		UPBInventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
			for (UPBItemModInstance* Mod : Instance->GetItemMods())
			{
				WroteSomething |= Channel->ReplicateSubobject(Mod, *Bunch, *RepFlags);
			}
		}
	}

	return WroteSomething;
}

void UPBInventoryComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing UPBInventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FPBInventoryEntry& Entry : InventoryList.Entries)
		{
			UPBInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
				for (UPBItemModInstance* Mod : Instance->GetItemMods())
				{
					AddReplicatedSubObject(Mod);
				}
			}
		}
	}
}


