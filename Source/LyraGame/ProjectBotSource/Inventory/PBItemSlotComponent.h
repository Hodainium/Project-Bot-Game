// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Engine/Classes/Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PBInventoryStructs.h"
#include "PBItemSlotComponent.generated.h"


class UPBWeaponItemDefinition;
struct FPBInventoryEntry;
class UPBEquipmentManagerComponent;
class UPBEquipmentInstance;
class UPBInventoryItemInstance;

// Delegate signature
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPBOnReceivedServerSwapData);

UENUM(BlueprintType)
enum class EPBInventorySlotType : uint8
{
	Weapon_L,
	Weapon_R,
	Temporary,
	UseItem,
	Consumable,
	Invalid
};
ENUM_RANGE_BY_FIRST_AND_LAST(EPBInventorySlotType, EPBInventorySlotType::Weapon_L, EPBInventorySlotType::Invalid);

USTRUCT(BlueprintType)
struct FPBInventorySlotIndex
{
	GENERATED_BODY()

public:

	FPBInventorySlotIndex()
	{
		SlotType = EPBInventorySlotType::Invalid;
		SlotIndex = 255;
	}

	bool operator==(const FPBInventorySlotIndex& Other) const
	{
		return Equals(Other);
	}

	bool operator!=(const FPBInventorySlotIndex& Other) const
	{
		return !Equals(Other);
	}

	bool Equals(const FPBInventorySlotIndex& Other) const
	{
		return (SlotType == Other.SlotType) && (SlotIndex == Other.SlotIndex);
	}

	bool IsValid() const
	{
		return SlotType != EPBInventorySlotType::Invalid;
	}

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	EPBInventorySlotType SlotType;

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	uint8 SlotIndex;
};

USTRUCT(BlueprintType)
struct FPBNullEquipmentEntry
{
	GENERATED_BODY()

public:

	FPBNullEquipmentEntry()
	{
		LeftWeaponDefinition = nullptr;
		RightWeaponDefinition = nullptr;
		StackNumber = 0;
	}

	FPBNullEquipmentEntry(UPBWeaponItemDefinition* LeftWeaponDef, UPBWeaponItemDefinition* RightWeaponDef)
	{
		LeftWeaponDefinition = LeftWeaponDef;
		RightWeaponDefinition = RightWeaponDef;
		StackNumber = 1;
	}

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	TObjectPtr<UPBWeaponItemDefinition> LeftWeaponDefinition;

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	TObjectPtr<UPBWeaponItemDefinition> RightWeaponDefinition;

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	uint8 StackNumber;

	bool operator==(const FPBNullEquipmentEntry& Other) const
	{
		return (LeftWeaponDefinition == Other.LeftWeaponDefinition) && (RightWeaponDefinition == Other.RightWeaponDefinition);
	}
};



//The item slot component is in charge of equipping items
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LYRAGAME_API UPBItemSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPBItemSlotComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Slots")
	void RequestSwapOperation(FPBInventorySlotIndex SourceIndex, FPBInventorySlotIndex TargetIndex);

	UFUNCTION(BlueprintCallable, Category = "Slots")
	void CycleActiveSlotForward(EPBInventorySlotType SlotType);

	UFUNCTION(BlueprintCallable, Category = "Slots")
	void CycleActiveSlotBackward(EPBInventorySlotType SlotType);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Slots")
	void SetTwoHanding(bool bInTwoHanding);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Slots")
	void SetActiveSlotIndexForEnum(EPBInventorySlotType SlotType, int32 NewIndex);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Slots")
	void Client_OpenInventory();

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	TArray<UPBInventoryItemInstance*> GetSlotsForEnum(EPBInventorySlotType SlotType) const
	{
		return GetSlotStructForEnum_Const(SlotType).SlotArray;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UPBInventoryItemInstance* GetItemAtIndex(FPBInventorySlotIndex Index) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	int32 GetNumSlotsForEnum(EPBInventorySlotType SlotType) const
	{
		return GetSlotStructForEnum_Const(SlotType).NumSlots;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	int32 GetActiveSlotIndexForEnum(EPBInventorySlotType SlotType) const
	{
		return GetSlotStructForEnum_Const(SlotType).ActiveSlotIndex;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UPBInventoryItemInstance* GetActiveSlotItem(EPBInventorySlotType SlotType) const;

	UFUNCTION(BlueprintCallable, Category = "ItemSlots|Net")
	bool GetIsTwoHanding() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	int32 GetNextFreeItemSlot(EPBInventorySlotType SlotType) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool TryFindIndexForItem(UPBInventoryItemInstance* ItemToFind, FPBInventorySlotIndex& OutIndex);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetNumSlotsForEnum(EPBInventorySlotType SlotType, int32 InNum);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddItemToSlot(EPBInventorySlotType SlotType, int32 SlotIndex, UPBInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddNullEquipment(FPBNullEquipmentEntry InNullEquipment);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void RemoveNullEquipment(FPBNullEquipmentEntry InNullEquipment);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void RemoveItemAtSlotIndex(EPBInventorySlotType SlotType, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool TryRemoveItemFromSlots(UPBInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "ItemSlots|Net")
	bool GetIsPendingServerConfirmation();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintAssignable)
	FPBOnReceivedServerSwapData OnReceivedServerSwapData;

	

private:

	void UnequipItemInSlot(EPBInventorySlotType SlotType);
	void EquipItemInSlot(EPBInventorySlotType SlotType);

	void HandleNullEquipmentChange();

	UPBEquipmentManagerComponent* FindEquipmentComponent() const;

	void Handle_OnRep_SlotsChanged(EPBInventorySlotType SlotType, const FPBInventorySlotStruct& PreviousValue);

	void Handle_OnRep_NumSlotsChanged(EPBInventorySlotType SlotType, const FPBInventorySlotStruct& PreviousValue);

	void Handle_OnRep_ActiveSlotIndexChanged(EPBInventorySlotType SlotType, const FPBInventorySlotStruct& PreviousValue);

protected:

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void Server_SwapSlots(FPBInventorySlotIndex SourceIndex, FPBInventorySlotIndex TargetIndex);

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = Inventory)
	void Client_SwapSlots(bool bSuccessful);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool AreSlotIndexesCompatible(FPBInventorySlotIndex SourceIndex, FPBInventorySlotIndex TargetIndex) const;

	//Checks to see if index is compatible with slot type.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Inventory)
	bool IsIndexCompatibleWithSlotType(FPBInventorySlotIndex Index, EPBInventorySlotType SlotType) const;

	//Flag that is set and removed when sending a server swap request and recieving confirmation
	UPROPERTY()
	bool IsPendingServerConfirmation;
	//The remaining slots that need to be checked after network update
	UPROPERTY()
	TArray<FPBInventorySlotIndex> PendingSlotData;

	UPROPERTY(EditDefaultsOnly, Category = "ItemSlots|Defaults")
	int WeaponLStartingSlots;

	UPROPERTY(EditDefaultsOnly, Category = "ItemSlots|Defaults")
	int WeaponRStartingSlots;

	UPROPERTY(EditDefaultsOnly, Category = "ItemSlots|Defaults")
	int TemporaryStartingSlots;

	UPROPERTY(EditDefaultsOnly, Category = "ItemSlots|Defaults")
	int UseItemStartingSlots;

	UPROPERTY(EditDefaultsOnly, Category = "ItemSlots|Defaults")
	int ConsumableStartingSlots;

	UPROPERTY(ReplicatedUsing = OnRep_SlotStruct_Weapon_L)
	FPBInventorySlotStruct SlotStruct_Weapon_L;

	UFUNCTION()
	void OnRep_bTwoHanding();

	UPROPERTY(ReplicatedUsing = OnRep_bTwoHanding)
	bool bTwoHanding;

	UFUNCTION()
	void OnRep_SlotStruct_Weapon_L(FPBInventorySlotStruct& PreviousValue);

	UPROPERTY(ReplicatedUsing = OnRep_SlotStruct_Weapon_R)
	FPBInventorySlotStruct SlotStruct_Weapon_R;

	UFUNCTION()
	void OnRep_SlotStruct_Weapon_R(FPBInventorySlotStruct& PreviousValue);

	UPROPERTY(ReplicatedUsing = OnRep_SlotStruct_Temporary)
	FPBInventorySlotStruct SlotStruct_Temporary;

	UFUNCTION()
	void OnRep_SlotStruct_Temporary(FPBInventorySlotStruct& PreviousValue);

	UPROPERTY(ReplicatedUsing = OnRep_SlotStruct_UseItem)
	FPBInventorySlotStruct SlotStruct_UseItem;

	UFUNCTION()
	void OnRep_SlotStruct_UseItem(FPBInventorySlotStruct& PreviousValue);

	UPROPERTY(ReplicatedUsing = OnRep_SlotStruct_Consumable)
	FPBInventorySlotStruct SlotStruct_Consumable;

	UFUNCTION()
	void OnRep_SlotStruct_Consumable(FPBInventorySlotStruct& PreviousValue);

	UPROPERTY()
	FPBInventorySlotStruct SlotStruct_Invalid;

	/*UPROPERTY(ReplicatedUsing = OnRep_SlotStruct_UseItem)
	FPBInventorySlotStruct SlotStruct_Core;

	UFUNCTION()
	void OnRep_SlotStruct_Core(FPBInventorySlotStruct& PreviousValue);*/

	UPROPERTY(Replicated)
	TArray<FPBNullEquipmentEntry> NullEquipmentStack;

	UFUNCTION()
	FPBInventorySlotStruct& GetSlotStructForEnum(EPBInventorySlotType SlotType);

	UFUNCTION()
	const FPBInventorySlotStruct& GetSlotStructForEnum_Const(EPBInventorySlotType SlotType) const;
};

USTRUCT(BlueprintType)
struct FPBItemSlotsChangedMessage
{
	GENERATED_BODY()

	FPBItemSlotsChangedMessage() : SlotType(EPBInventorySlotType::Weapon_L)
	{
	}

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	EPBInventorySlotType SlotType;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TArray<TObjectPtr<UPBInventoryItemInstance>> Slots;
};

USTRUCT(BlueprintType)
struct FPBItemSlotsActiveIndexChangedMessage
{
	GENERATED_BODY()

	FPBItemSlotsActiveIndexChangedMessage() : SlotType(EPBInventorySlotType::Weapon_L)
	{
	}

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	EPBInventorySlotType SlotType;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	int32 ActiveIndex = 0;
};

USTRUCT(BlueprintType)
struct FPBItemSlotsNumSlotsChangedMessage
{
	GENERATED_BODY()

	FPBItemSlotsNumSlotsChangedMessage(): SlotType(EPBInventorySlotType::Weapon_L)
	{
	}

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	EPBInventorySlotType SlotType;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	int32 NumSlots = 0;
};

USTRUCT(BlueprintType)
struct FPBTwoHandingChangedMessage
{
	GENERATED_BODY()

	FPBTwoHandingChangedMessage() : TwoHanding(false)
	{
	}

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	bool TwoHanding;
};
