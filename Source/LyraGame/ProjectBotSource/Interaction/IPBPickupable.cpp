// Copyright Epic Games, Inc. All Rights Reserved.

#include "IPBPickupable.h"

#include "AsyncMixin.h"
#include "GameFramework/Actor.h"
#include "Engine/AssetManager.h"
#include "Inventory/IPickupable.h"
#include "Logging/StructuredLog.h"
#include "ProjectBotSource/Inventory/PBInventoryComponent.h"
#include "ProjectBotSource/Inventory/PBItemDefinition.h"
#include "ProjectBotSource/Inventory/PBItemInstance.h"
#include "ProjectBotSource/Inventory/PBItemSlotComponent.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"
#include "UObject/ScriptInterface.h"

class UActorComponent;

UPBPickupableStatics::UPBPickupableStatics()
	: Super(FObjectInitializer::Get())
{
}

TScriptInterface<IPBPickupable> UPBPickupableStatics::GetFirstPickupableFromActor(AActor* Actor)
{
	// If the actor is directly pickupable, return that.
	TScriptInterface<IPBPickupable> PickupableActor(Actor);
	if (PickupableActor)
	{
		return PickupableActor;
	}

	// If the actor isn't pickupable, it might have a component that has a pickupable interface.
	TArray<UActorComponent*> PickupableComponents = Actor ? Actor->GetComponentsByInterface(UPBPickupable::StaticClass()) : TArray<UActorComponent*>();
	if (PickupableComponents.Num() > 0)
	{
		// Get first pickupable, if the user needs more sophisticated pickup distinction, will need to be solved elsewhere.
		return TScriptInterface<IPBPickupable>(PickupableComponents[0]);
	}

	return TScriptInterface<IPBPickupable>();
}

void UPBPickupableStatics::AddPickupToInventory(UPBInventoryComponent* InventoryComponent, TScriptInterface<IPBPickupable> Pickup)
{
	if (InventoryComponent && Pickup)
	{
		const FPBInventoryPickup& PickupInventory = Pickup->GetPickupInventory();

		for (const FPBPickupTemplate& Template : PickupInventory.Templates)
		{
			UPBItemDefinition* ItemDefRef = Template.ItemDef.LoadSynchronous();
			InventoryComponent->AddItemDefinition(ItemDefRef, Template.StackCount);
		}

		for (const FPBPickupInstance& Instance : PickupInventory.Instances)
		{
			InventoryComponent->AddItemInstance(Instance.Item);
		}
	}
}

void UPBPickupableStatics::PushPickupToPlayer(APawn* PlayerPawn, TScriptInterface<IPBPickupable> Pickup)
{
	UPBInventoryComponent* InventoryComponent = PlayerPawn->GetComponentByClass<UPBInventoryComponent>();
	UPBItemSlotComponent* SlotComponent = PlayerPawn->GetComponentByClass<UPBItemSlotComponent>();
	
	if (InventoryComponent && SlotComponent && Pickup)
	{
		const FPBInventoryPickup& PickupInventory = Pickup->GetPickupInventory();

		for (const FPBPickupTemplate& Template : PickupInventory.Templates)
		{
			UPBItemDefinition* ItemDefRef = Template.ItemDef.LoadSynchronous();
			UE_LOGFMT(LogPBGame, Error, "WE ARE SYNC LOADING IN PUSHITEM TO PLAYER!!!!!!!!!!!!!!!!! FIX ASAP");

			//FAsyncMixin::AsyncLoad(Template.ItemDef, )
			//FStreamableManager::RequestAsyncLoad

			if(ItemDefRef->ItemType == EPBItemType::Weapon)
			{
				UPBInventoryItemInstance* ItemInstanceToAdd = InventoryComponent->AddItemDefinition(ItemDefRef, Template.StackCount);

				if(SlotComponent->GetSlotsForEnum(EPBInventorySlotType::Temporary)[0] != nullptr)
				{
					UE_LOGFMT(LogPBGame, Warning, "Dropping extra gun from pickup not yet implemented");
				}
				else
				{
					SlotComponent->AddItemToSlot(EPBInventorySlotType::Temporary, 0, ItemInstanceToAdd);
				}
			}
			else
			{
				UE_LOGFMT(LogPBGame, Error, "Dropping extra gun from pickup not yet implemented");
			}

			
			return;
		}

		int i = 0;

		for (const FPBPickupInstance& Instance : PickupInventory.Instances)
		{
			/*InventoryComponent->AddItemInstance(Instance.Item);
			SlotComponent->AddItemToSlot(EPBInventorySlotType::Temporary, 0, Instance.Item);*/

			UE_LOGFMT(LogPBGame, Warning, "Loop: {num}", i);

			if (Instance.Item->GetItemTypeEnum() == EPBItemType::Weapon)
			{
				UPBInventoryItemInstance* Result = InventoryComponent->AddItemInstance(Instance.Item);

				if (SlotComponent->GetSlotsForEnum(EPBInventorySlotType::Temporary)[0] != nullptr)
				{
					UE_LOGFMT(LogPBGame, Warning, "Dropping extra gun from pickup not yet implemented");
				}
				else
				{
					SlotComponent->AddItemToSlot(EPBInventorySlotType::Temporary, 0, Result);
				}
			}

			return;
		}
	}
}

void UPBPickupableStatics::OnWeaponLoad()
{
}
