// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Inventory/PBItemSlotComponent.h"
#include "NativeGameplayTags.h"
#include "ProjectBotSource/Equipment/PBEquipmentManagerComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "ProjectBotSource/Inventory/PBItemDefinition.h"
#include "ProjectBotSource/Inventory/PBItemInstance.h"
#include "ProjectBotSource/Inventory/ItemTypes/PBWeaponItemDefinition.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"
#include "ProjectBotSource/Tags/PB_Tags.h"

UPBItemSlotComponent::UPBItemSlotComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);

	IsPendingServerConfirmation = false;
}

void UPBItemSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		SetNumSlotsForEnum(EPBInventorySlotType::Weapon_L, WeaponLStartingSlots);
		SetNumSlotsForEnum(EPBInventorySlotType::Weapon_R, WeaponRStartingSlots);
		SetNumSlotsForEnum(EPBInventorySlotType::Temporary, TemporaryStartingSlots);
		SetNumSlotsForEnum(EPBInventorySlotType::Item, ItemStartingSlots);
	}
}

void UPBItemSlotComponent::RequestSwapOperation(FPBInventorySlotIndex SourceIndex, FPBInventorySlotIndex TargetIndex)
{
	IsPendingServerConfirmation = true;
	Server_SwapSlots(SourceIndex, TargetIndex);
}

void UPBItemSlotComponent::CycleActiveSlotForward(EPBInventorySlotType SlotType)
{
	FPBInventorySlotStruct& Slots = GetSlotStructForEnum(SlotType);

	if (Slots.SlotArray.Num() < 2)
	{
		return;
	}

	const int32 OldIndex = (Slots.ActiveSlotIndex < 0 ? Slots.SlotArray.Num() - 1 : Slots.ActiveSlotIndex);
	int32 NewIndex = Slots.ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex + 1) % Slots.SlotArray.Num();
		if (Slots.SlotArray[NewIndex] != nullptr && (NewIndex != OldIndex))
		{
			SetActiveSlotIndexForEnum(SlotType, NewIndex);
			return;
		}
	} while (NewIndex != OldIndex);

	//Get an empty slot. No other occupied slots are available
	SetActiveSlotIndexForEnum(SlotType, GetNextFreeItemSlot(SlotType));
}

void UPBItemSlotComponent::CycleActiveSlotBackward(EPBInventorySlotType SlotType)
{
	FPBInventorySlotStruct& Slots = GetSlotStructForEnum(SlotType);

	if (Slots.SlotArray.Num() < 2)
	{
		return;
	}

	const int32 OldIndex = (Slots.ActiveSlotIndex < 0 ? Slots.SlotArray.Num() - 1 : Slots.ActiveSlotIndex);
	int32 NewIndex = Slots.ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex - 1 + Slots.SlotArray.Num()) % Slots.SlotArray.Num();
		if (Slots.SlotArray[NewIndex] != nullptr)
		{
			SetActiveSlotIndexForEnum(SlotType, NewIndex);
			return;
		}
	} while (NewIndex != OldIndex);

	//Get an empty slot. No other occupied slots are available
	SetActiveSlotIndexForEnum(SlotType, GetNextFreeItemSlot(SlotType));
}

UPBInventoryItemInstance* UPBItemSlotComponent::GetItemAtIndex(FPBInventorySlotIndex Index) const
{
	const FPBInventorySlotStruct& Slots = GetSlotStructForEnum_Const(Index.SlotType);

	return Slots.SlotArray.IsValidIndex(Index.SlotIndex) ? Slots.SlotArray[Index.SlotIndex] : nullptr;
}

void UPBItemSlotComponent::SetActiveSlotIndexForEnum_Implementation(EPBInventorySlotType SlotType, int32 NewIndex)
{
	FPBInventorySlotStruct& Slots = GetSlotStructForEnum(SlotType);


	if (Slots.SlotArray.IsValidIndex(NewIndex) && (Slots.ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot(SlotType);

		Slots.ActiveSlotIndex = NewIndex;

		EquipItemInSlot(SlotType);
		Handle_OnRep_ActiveSlotIndexChanged(SlotType);
	}
}

UPBInventoryItemInstance* UPBItemSlotComponent::GetActiveSlotItem(EPBInventorySlotType SlotType) const
{
	const FPBInventorySlotStruct& Slots = GetSlotStructForEnum_Const(SlotType);

	return Slots.SlotArray.IsValidIndex(Slots.ActiveSlotIndex) ? Slots.SlotArray[Slots.ActiveSlotIndex] : nullptr;
}

int32 UPBItemSlotComponent::GetNextFreeItemSlot(EPBInventorySlotType SlotType) const
{
	const FPBInventorySlotStruct& Slots = GetSlotStructForEnum_Const(SlotType);

	int32 SlotIndex = 0;
	for (TObjectPtr<UPBInventoryItemInstance> ItemPtr : Slots.SlotArray)
	{
		if (ItemPtr == nullptr)
		{
			return SlotIndex;
		}
		++SlotIndex;
	}

	return INDEX_NONE;
}

bool UPBItemSlotComponent::TryFindIndexForItem(UPBInventoryItemInstance* ItemToFind, FPBInventorySlotIndex& OutIndex)
{
	for (EPBInventorySlotType SlotType : TEnumRange<EPBInventorySlotType>())
	{
		const FPBInventorySlotStruct& Slots = GetSlotStructForEnum_Const(SlotType);

		for (int i = 0; i < Slots.SlotArray.Num(); i++)
		{
			if(ItemToFind == Slots.SlotArray[i])
			{
				OutIndex.SlotIndex = i;
				OutIndex.SlotType = SlotType;

				return true;
			}
		}
	}

	return false;
}

void UPBItemSlotComponent::Server_SwapSlots_Implementation(FPBInventorySlotIndex SourceIndex, FPBInventorySlotIndex TargetIndex)
{
	FPBInventorySlotStruct& SourceSlots = GetSlotStructForEnum(SourceIndex.SlotType);
	FPBInventorySlotStruct& TargetSlots = GetSlotStructForEnum(TargetIndex.SlotType);

	bool bWasSuccessful = false;

	if (SourceSlots.SlotArray.IsValidIndex(SourceIndex.SlotIndex) && TargetSlots.SlotArray.IsValidIndex(TargetIndex.SlotIndex))
	{
		UPBInventoryItemInstance* TempInstance = TargetSlots.SlotArray[TargetIndex.SlotIndex];

		TargetSlots.SlotArray[TargetIndex.SlotIndex] = SourceSlots.SlotArray[SourceIndex.SlotIndex];

		SourceSlots.SlotArray[SourceIndex.SlotIndex] = TempInstance;

		Handle_OnRep_SlotsChanged(SourceIndex.SlotType);
		Handle_OnRep_SlotsChanged(TargetIndex.SlotType);

		if(SourceSlots.ActiveSlotIndex == SourceIndex.SlotIndex)
		{
			UnequipItemInSlot(SourceIndex.SlotType);
			EquipItemInSlot(SourceIndex.SlotType);
		}

		if (TargetSlots.ActiveSlotIndex == TargetIndex.SlotIndex)
		{
			UnequipItemInSlot(TargetIndex.SlotType);
			EquipItemInSlot(TargetIndex.SlotType);
		}

		bWasSuccessful = true;
	}

	Client_SwapSlots_Implementation(bWasSuccessful);
}

void UPBItemSlotComponent::Client_SwapSlots_Implementation(bool bWasSuccessful)
{
	IsPendingServerConfirmation = false;

	//Broadcast a message here
	if(OnReceivedServerSwapConfirmation.IsBound())
	{
		OnReceivedServerSwapConfirmation.Broadcast(bWasSuccessful);
	}
}

void UPBItemSlotComponent::SetNumSlotsForEnum(EPBInventorySlotType SlotType, int32 InNum)
{
	FPBInventorySlotStruct& Slots = GetSlotStructForEnum(SlotType);

	if(Slots.SlotArray.Num() == InNum)
	{
		return;
	}

	if (Slots.SlotArray.Num() < InNum)
	{
		Slots.SlotArray.AddDefaulted(InNum - Slots.SlotArray.Num());
	}
	else if (Slots.SlotArray.Num() > InNum)
	{
		//Downsizing array not yet implemented
		UE_LOGFMT(LogPBGame, Warning, "Removing items but not yet dropping. Need to implement dropping items");
		//TODO maybe we can just send a gameplay event because dropping is handled via gas

		for (int i = 0; i < (Slots.SlotArray.Num() - InNum); i++)
		{
			int currentIdx = Slots.SlotArray.Num() - 1 - i;

			//Should drop items at these indexes
			Slots.SlotArray[currentIdx] = nullptr;
		}

		Slots.SlotArray.SetNum(InNum, true);

		//Set active index to be within bounds
		if(Slots.ActiveSlotIndex >= InNum)
		{
			SetActiveSlotIndexForEnum(SlotType, InNum - 1);
		}

		UE_LOGFMT(LogPBGame, Warning, "Set num slots New raw size: {size}", Slots.SlotArray.Num());
	}

	UE_LOGFMT(LogPBGame, Warning, "Num slots requesting size {size}", InNum);
	UE_LOGFMT(LogPBGame, Warning, "Num slots og size {size}", Slots.SlotArray.Num());

	UE_LOGFMT(LogPBGame, Warning, "Set num slots == {NUM}", InNum);

	Slots.NumSlots = InNum;
	
	Handle_OnRep_NumSlotsChanged(SlotType);
	Handle_OnRep_SlotsChanged(SlotType);
}

void UPBItemSlotComponent::AddItemToSlot(EPBInventorySlotType SlotType, int32 SlotIndex, UPBInventoryItemInstance* Item)
{
	FPBInventorySlotStruct& Slots = GetSlotStructForEnum(SlotType);

	if (Slots.SlotArray.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots.SlotArray[SlotIndex] == nullptr)
		{
			UE_LOGFMT(LogPBGame, Warning, "Changing slot at index: {idx}", SlotIndex);
			Slots.SlotArray[SlotIndex] = Item;
			Handle_OnRep_SlotsChanged(SlotType);
		}
	}
}

void UPBItemSlotComponent::RemoveItemAtSlotIndex(EPBInventorySlotType SlotType, int32 SlotIndex)
{
	FPBInventorySlotStruct& Slots = GetSlotStructForEnum(SlotType);

	UPBInventoryItemInstance* Result = nullptr;

	//if (Slots.ActiveSlotIndex == SlotIndex)
	//{
	//	UnequipItemInSlot(SlotType);
	//	//Slots.ActiveSlotIndex = -1;
	//}

	if (Slots.SlotArray.IsValidIndex(SlotIndex))
	{
		Result = Slots.SlotArray[SlotIndex];

		if (Result != nullptr)
		{
			Slots.SlotArray[SlotIndex] = nullptr;
			Handle_OnRep_SlotsChanged(SlotType);
		}
	}

	//This should be safe to do after
	if(Slots.ActiveSlotIndex == SlotIndex)
	{
		UnequipItemInSlot(SlotType);

		//Now equip the null weapon
		EquipItemInSlot(SlotType);
	}
}

void UPBItemSlotComponent::TryRemoveItemFromSlots(UPBInventoryItemInstance* Item)
{
	FPBInventorySlotIndex Index;
	if(TryFindIndexForItem(Item, Index))
	{
		RemoveItemAtSlotIndex(Index.SlotType, Index.SlotIndex);
	}
}

bool UPBItemSlotComponent::GetIsPendingServerConfirmation()
{
	return IsPendingServerConfirmation;
}

void UPBItemSlotComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, SlotStruct_Weapon_L);
	DOREPLIFETIME(ThisClass, SlotStruct_Weapon_R);
	DOREPLIFETIME(ThisClass, SlotStruct_Temporary);
	DOREPLIFETIME(ThisClass, SlotStruct_Item);
	DOREPLIFETIME(ThisClass, NullEquipmentStack);
}

void UPBItemSlotComponent::UnequipItemInSlot(EPBInventorySlotType SlotType)
{
	if (UPBEquipmentManagerComponent* EquipmentManager = FindEquipmentComponent())
	{
		UPBEquipmentInstance* EquippedItem = GetSlotStructForEnum(SlotType).EquippedItem;

		if (EquippedItem != nullptr)
		{
			EquipmentManager->UnequipItem(EquippedItem);
			EquippedItem = nullptr;
		}
	}
}

void UPBItemSlotComponent::EquipItemInSlot(EPBInventorySlotType SlotType)
{
	FPBInventorySlotStruct& Slots = GetSlotStructForEnum(SlotType);

	check(Slots.SlotArray.IsValidIndex(Slots.ActiveSlotIndex));
	//check(Slots.EquippedItem == nullptr);

	if (UPBInventoryItemInstance* SlotItem = Slots.SlotArray[Slots.ActiveSlotIndex])
	{

		EPBItemType DefItemType = SlotItem->GetItemDefinition()->ItemType;

		switch (DefItemType)
		{
		case EPBItemType::Weapon:
		{
			if (UPBWeaponItemDefinition* ItemDef = Cast<UPBWeaponItemDefinition>(SlotItem->GetItemDefinition()))
			{
				if (UPBEquipmentManagerComponent* EquipmentManager = FindEquipmentComponent())
				{
					UE_LOGFMT(LogPBGame, Warning, "EquipComp is valid");

					//Slots.EquippedItem = EquipmentManager->EquipItem(ItemDef->WeaponDefinition);
					Slots.EquippedItem = EquipmentManager->EquipItem(ItemDef->WeaponDefinition, SlotItem);
				}
			}

			break;
		}
		default:
		{
			UE_LOG(LogPBGame, Error, TEXT("ITEMSLOTCOMP::BrUPB you didn't include equipItemInSlot logic. For item: %s"), *SlotItem->GetItemDefinition()->ItemName.ToString());
		}
		}
	}
	else
	{
		//Search null equipment array for valid weapon def if so equip it.

		if(NullEquipmentStack.Num() > 0) 
		{
			UPBWeaponItemDefinition* NullWeaponDef = nullptr; 

			for(int i = 0; i < NullEquipmentStack.Num(); i++)
			{
				if(NullEquipmentStack[NullEquipmentStack.Num() - 1 - i].WeaponDefinition != nullptr)
				{
					NullWeaponDef = NullEquipmentStack[NullEquipmentStack.Num() - 1 - i].WeaponDefinition;
					break;
				}
			}

			if(NullWeaponDef)
			{
				if (UPBEquipmentManagerComponent* EquipmentManager = FindEquipmentComponent())
				{
					//TODO create variants within null equipment entry for both left and right weapon slot

					//Slots.EquippedItem = EquipmentManager->EquipItem(ItemDef->WeaponDefinition);
					Slots.EquippedItem = EquipmentManager->EquipItem(NullWeaponDef->WeaponDefinition, SlotItem);
				}
			}
		}
	}
}

void UPBItemSlotComponent::HandleNullEquipmentChange()
{
	FPBInventorySlotStruct& LeftSlots = GetSlotStructForEnum(EPBInventorySlotType::Weapon_L);
	FPBInventorySlotStruct& RightSlots = GetSlotStructForEnum(EPBInventorySlotType::Weapon_R);

	if(LeftSlots.SlotArray.IsValidIndex(LeftSlots.ActiveSlotIndex) && LeftSlots.SlotArray[LeftSlots.ActiveSlotIndex] == nullptr)
	{
		UnequipItemInSlot(EPBInventorySlotType::Weapon_L);
		EquipItemInSlot(EPBInventorySlotType::Weapon_L);
	}

	if (RightSlots.SlotArray.IsValidIndex(RightSlots.ActiveSlotIndex) && RightSlots.SlotArray[RightSlots.ActiveSlotIndex] == nullptr)
	{
		UnequipItemInSlot(EPBInventorySlotType::Weapon_R);
		EquipItemInSlot(EPBInventorySlotType::Weapon_R);
	}
}

UPBEquipmentManagerComponent* UPBItemSlotComponent::FindEquipmentComponent() const
{

	if (APawn* OwningPawn = Cast<APawn>(GetOwner()))
	{
		return OwningPawn->FindComponentByClass<UPBEquipmentManagerComponent>();
	}

	return nullptr;
}

void UPBItemSlotComponent::Handle_OnRep_SlotsChanged(EPBInventorySlotType SlotType)
{
	FPBItemSlotsChangedMessage Message;
	Message.Owner = GetOwner();
	Message.Slots = GetSlotStructForEnum_Const(SlotType).SlotArray;
	Message.SlotType = SlotType;

	UE_LOGFMT(LogPBGame, Warning, "This is happening!");

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(PB_ItemSlots_Tags::TAG_ITEMSLOTS_MESSAGE_SLOTSCHANGED, Message);
}

void UPBItemSlotComponent::Handle_OnRep_NumSlotsChanged(EPBInventorySlotType SlotType)
{
	FPBItemSlotsNumSlotsChangedMessage Message;
	Message.Owner = GetOwner();
	Message.NumSlots = GetSlotStructForEnum_Const(SlotType).NumSlots;
	Message.SlotType = SlotType;

	UE_LOGFMT(LogPBGame, Error, "Sending numslots message: {typ}", Message.NumSlots);

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(PB_ItemSlots_Tags::TAG_ITEMSLOTS_MESSAGE_NUMSLOTSCHANGED, Message);
}

void UPBItemSlotComponent::Handle_OnRep_ActiveSlotIndexChanged(EPBInventorySlotType SlotType)
{
	FPBItemSlotsActiveIndexChangedMessage Message;
	Message.Owner = GetOwner();
	Message.ActiveIndex = GetSlotStructForEnum_Const(SlotType).ActiveSlotIndex;
	Message.SlotType = SlotType;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(PB_ItemSlots_Tags::TAG_ITEMSLOTS_MESSAGE_ACTIVEINDEXCHANGED, Message);
}

void UPBItemSlotComponent::OnRep_SlotStruct_Weapon_L(FPBInventorySlotStruct& PreviousValue)
{
	if (SlotStruct_Weapon_L.SlotArray != PreviousValue.SlotArray)
	{
		Handle_OnRep_SlotsChanged(EPBInventorySlotType::Weapon_L);
	}

	if (SlotStruct_Weapon_L.NumSlots != PreviousValue.NumSlots)
	{
		Handle_OnRep_NumSlotsChanged(EPBInventorySlotType::Weapon_L);
	}

	if (SlotStruct_Weapon_L.ActiveSlotIndex != PreviousValue.ActiveSlotIndex)
	{
		Handle_OnRep_ActiveSlotIndexChanged(EPBInventorySlotType::Weapon_L);
	}
}

void UPBItemSlotComponent::OnRep_SlotStruct_Weapon_R(FPBInventorySlotStruct& PreviousValue)
{
	if (SlotStruct_Weapon_R.SlotArray != PreviousValue.SlotArray)
	{
		Handle_OnRep_SlotsChanged(EPBInventorySlotType::Weapon_R);
	}

	if (SlotStruct_Weapon_R.NumSlots != PreviousValue.NumSlots)
	{
		Handle_OnRep_NumSlotsChanged(EPBInventorySlotType::Weapon_R);
	}

	if (SlotStruct_Weapon_R.ActiveSlotIndex != PreviousValue.ActiveSlotIndex)
	{
		Handle_OnRep_ActiveSlotIndexChanged(EPBInventorySlotType::Weapon_R);
	}
}

void UPBItemSlotComponent::OnRep_SlotStruct_Temporary(FPBInventorySlotStruct& PreviousValue)
{
	if (SlotStruct_Temporary.SlotArray != PreviousValue.SlotArray)
	{
		Handle_OnRep_SlotsChanged(EPBInventorySlotType::Temporary);
	}

	if (SlotStruct_Temporary.NumSlots != PreviousValue.NumSlots)
	{
		Handle_OnRep_NumSlotsChanged(EPBInventorySlotType::Temporary);
	}

	if (SlotStruct_Temporary.ActiveSlotIndex != PreviousValue.ActiveSlotIndex)
	{
		Handle_OnRep_ActiveSlotIndexChanged(EPBInventorySlotType::Temporary);
	}
}

void UPBItemSlotComponent::OnRep_SlotStruct_Item(FPBInventorySlotStruct& PreviousValue)
{
	if (SlotStruct_Temporary.SlotArray != PreviousValue.SlotArray)
	{
		Handle_OnRep_SlotsChanged(EPBInventorySlotType::Item);
	}

	if (SlotStruct_Temporary.NumSlots != PreviousValue.NumSlots)
	{
		Handle_OnRep_NumSlotsChanged(EPBInventorySlotType::Item);
	}

	if (SlotStruct_Temporary.ActiveSlotIndex != PreviousValue.ActiveSlotIndex)
	{
		Handle_OnRep_ActiveSlotIndexChanged(EPBInventorySlotType::Item);
	}
}

void UPBItemSlotComponent::AddNullEquipment(UPBWeaponItemDefinition* InEquipment)
{
	bool bFound = false;
	for (FPBNullEquipmentEntry& Entry : NullEquipmentStack)
	{
		if(Entry.WeaponDefinition == InEquipment)
		{
			Entry.StackNumber += 1;
			bFound = true;
			break;
		}
	}

	if(!bFound)
	{
		NullEquipmentStack.Add(FPBNullEquipmentEntry(InEquipment));
	}

	HandleNullEquipmentChange();
}

void UPBItemSlotComponent::RemoveNullEquipment(UPBWeaponItemDefinition* EquipmentToRemove)
{
	for (FPBNullEquipmentEntry& Entry : NullEquipmentStack)
	{
		if (Entry.WeaponDefinition == EquipmentToRemove)
		{
			Entry.StackNumber -= 1;
			HandleNullEquipmentChange();
			return;
		}
	}

	UE_LOGFMT(LogPBGame, Error, "Tried to remove null equipment that didnt exist");
}

//ALSO NEED TO CHANGE CONST VERSION
FPBInventorySlotStruct& UPBItemSlotComponent::GetSlotStructForEnum(EPBInventorySlotType SlotType)
{
	switch(SlotType)
	{
	case EPBInventorySlotType::Weapon_L:
		return SlotStruct_Weapon_L;
	case EPBInventorySlotType::Weapon_R:
		return SlotStruct_Weapon_R;
	case EPBInventorySlotType::Temporary:
		return SlotStruct_Temporary;
	case EPBInventorySlotType::Item:
		return SlotStruct_Item;
	default:
		{
			UE_LOGFMT(LogPBGame, Error, "Invalid enum in getslotstruct invcomp");
			return SlotStruct_Weapon_L;
		}
	}
}

//ALSO NEED TO CHANGE NONCONST VERSION
const FPBInventorySlotStruct& UPBItemSlotComponent::GetSlotStructForEnum_Const(EPBInventorySlotType SlotType) const
{
	switch (SlotType)
	{
	case EPBInventorySlotType::Weapon_L:
		return SlotStruct_Weapon_L;
	case EPBInventorySlotType::Weapon_R:
		return SlotStruct_Weapon_R;
	case EPBInventorySlotType::Temporary:
		return SlotStruct_Temporary;
	case EPBInventorySlotType::Item:
		return SlotStruct_Item;
	default:
	{
		UE_LOGFMT(LogPBGame, Error, "Invalid enum in getslotstruct invcomp");
		return SlotStruct_Weapon_L;
	}
	}
}

