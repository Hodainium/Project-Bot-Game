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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPBOnReceivedServerSwapConfirmation, bool, bApproved);

UENUM(BlueprintType)
enum class EPBInventorySlotType : uint8
{
	Weapon_L,
	Weapon_R,
	Temporary,
	Item
};
ENUM_RANGE_BY_FIRST_AND_LAST(EPBInventorySlotType, EPBInventorySlotType::Weapon_L, EPBInventorySlotType::Item);

USTRUCT(BlueprintType)
struct FPBNullEquipmentEntry
{
	GENERATED_BODY()

public:

	FPBNullEquipmentEntry()
	{
		WeaponDefinition = nullptr;
		StackNumber = 0;
	}

	FPBNullEquipmentEntry(UPBWeaponItemDefinition* WeaponDef)
	{
		WeaponDefinition = WeaponDef;
		StackNumber = 1;
	}

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	TObjectPtr<UPBWeaponItemDefinition> WeaponDefinition;

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	uint8 StackNumber;
};

USTRUCT(BlueprintType)
struct FPBInventorySlotIndex
{
	GENERATED_BODY()

public:

	FPBInventorySlotIndex()
	{
		SlotType = EPBInventorySlotType::Weapon_L;
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

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	EPBInventorySlotType SlotType;

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	uint8 SlotIndex;
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
	void SetActiveSlotIndexForEnum(EPBInventorySlotType SlotType, int32 NewIndex);

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

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	int32 GetNextFreeItemSlot(EPBInventorySlotType SlotType) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool TryFindIndexForItem(UPBInventoryItemInstance* ItemToFind, FPBInventorySlotIndex& OutIndex);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetNumSlotsForEnum(EPBInventorySlotType SlotType, int32 InNum);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddItemToSlot(EPBInventorySlotType SlotType, int32 SlotIndex, UPBInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddNullEquipment(UPBWeaponItemDefinition* InEquipment);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void RemoveNullEquipment(UPBWeaponItemDefinition* EquipmentToRemove);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void RemoveItemAtSlotIndex(EPBInventorySlotType SlotType, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void TryRemoveItemFromSlots(UPBInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "ItemSlots|Net")
	bool GetIsPendingServerConfirmation();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintAssignable)
	FPBOnReceivedServerSwapConfirmation OnReceivedServerSwapConfirmation;

	

private:

	void UnequipItemInSlot(EPBInventorySlotType SlotType);
	void EquipItemInSlot(EPBInventorySlotType SlotType);

	void HandleNullEquipmentChange();

	UPBEquipmentManagerComponent* FindEquipmentComponent() const;

	void Handle_OnRep_SlotsChanged(EPBInventorySlotType SlotType);

	void Handle_OnRep_NumSlotsChanged(EPBInventorySlotType SlotType);

	void Handle_OnRep_ActiveSlotIndexChanged(EPBInventorySlotType SlotType);

protected:

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void Server_SwapSlots(FPBInventorySlotIndex SourceIndex, FPBInventorySlotIndex TargetIndex);

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = Inventory)
	void Client_SwapSlots(bool bWasSuccessful);

	//Flag that is set and removed when sending a server swap request and recieving confirmation
	UPROPERTY()
	bool IsPendingServerConfirmation;

	UPROPERTY(EditDefaultsOnly, Category = "ItemSlots|Defaults")
	int WeaponLStartingSlots = 3;

	UPROPERTY(EditDefaultsOnly, Category = "ItemSlots|Defaults")
	int WeaponRStartingSlots = 3;

	UPROPERTY(EditDefaultsOnly, Category = "ItemSlots|Defaults")
	int TemporaryStartingSlots = 1;

	UPROPERTY(EditDefaultsOnly, Category = "ItemSlots|Defaults")
	int ItemStartingSlots = 1;

	UPROPERTY(ReplicatedUsing = OnRep_SlotStruct_Weapon_L)
	FPBInventorySlotStruct SlotStruct_Weapon_L;

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

	UPROPERTY(ReplicatedUsing = OnRep_SlotStruct_Item)
	FPBInventorySlotStruct SlotStruct_Item;

	UFUNCTION()
	void OnRep_SlotStruct_Item(FPBInventorySlotStruct& PreviousValue);

	/*UPROPERTY(ReplicatedUsing = OnRep_SlotStruct_Item)
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
