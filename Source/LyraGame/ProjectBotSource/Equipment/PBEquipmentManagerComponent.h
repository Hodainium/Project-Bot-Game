// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "AbilitySystem/LyraAbilitySet.h"
#include "PBEquipmentManagerComponent.generated.h"


class UPBInventoryItemInstance;
struct FPBEquipmentList;
class UPBEquipmentManagerComponent;
class UPBEquipmentDefinition;
class UPBEquipmentInstance;

/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FPBAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FPBAppliedEquipmentEntry()
	{}

	FString GetDebugString() const;

private:
	friend FPBEquipmentList;
	friend UPBEquipmentManagerComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TObjectPtr<UPBEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UPBEquipmentInstance> Instance = nullptr;

	// Authority-only list of granted handles
	UPROPERTY(NotReplicated)
	FLyraAbilitySet_GrantedHandles GrantedHandles;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FPBEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FPBEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FPBEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FPBAppliedEquipmentEntry, FPBEquipmentList>(Entries, DeltaParms, *this);
	}

	UPBEquipmentInstance* AddEntry(UPBEquipmentDefinition* EquipmentDefinition);
	void RemoveEntry(UPBEquipmentInstance* Instance);

private:
	ULyraAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UPBEquipmentManagerComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FPBAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FPBEquipmentList> : public TStructOpsTypeTraitsBase2<FPBEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};

/**
 * Manages equipment applied to a pawn
 */
UCLASS(BlueprintType, Const)
class LYRAGAME_API UPBEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPBEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());



	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UPBEquipmentInstance* EquipItem(UPBEquipmentDefinition* EquipmentDefinition, UObject* InInstigator);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UPBEquipmentInstance* EquipmentInstance);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPBEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UPBEquipmentInstance> InstanceType);

	/** Returns all equipped instances of a given type, or an empty array if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UPBEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UPBEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FPBEquipmentList EquipmentList;


};
