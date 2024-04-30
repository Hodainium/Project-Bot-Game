// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Equipment/PBEquipmentManagerComponent.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Engine/ActorChannel.h"
#include "ProjectBotSource/Equipment/PBEquipmentDefinition.h"
#include "ProjectBotSource/Equipment/PBEquipmentInstance.h"
#include "Net/UnrealNetwork.h"

class FLifetimeProperty;
struct FReplicationFlags;

//////////////////////////////////////////////////////////////////////
// FPBAppliedEquipmentEntry

FString FPBAppliedEquipmentEntry::GetDebugString() const
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(EquipmentDefinition.Get()));
}

//////////////////////////////////////////////////////////////////////
// FPBEquipmentList

void FPBEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FPBAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
	}
}

void FPBEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FPBAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FPBEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
}

ULyraAbilitySystemComponent* FPBEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<ULyraAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UPBEquipmentInstance* FPBEquipmentList::AddEntry(UPBEquipmentDefinition* EquipmentDefinition)
{
	UPBEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	TSubclassOf<UPBEquipmentInstance> InstanceType = EquipmentDefinition->InstanceType;

	if (InstanceType == nullptr)
	{
		InstanceType = UPBEquipmentInstance::StaticClass();
	}

	FPBAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UPBEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);  //@TODO: Using the actor instead of component as the outer due to UE-127172
	Result = NewEntry.Instance;

	if (ULyraAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (TObjectPtr<const ULyraAbilitySet> AbilitySet : EquipmentDefinition->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
		}
	}
	else
	{
		//@TODO: Warning logging?
	}

	Result->SpawnEquipmentActors(EquipmentDefinition->ActorsToSpawn);


	MarkItemDirty(NewEntry);

	return Result;
}

void FPBEquipmentList::RemoveEntry(UPBEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPBAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (ULyraAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();


			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

//////////////////////////////////////////////////////////////////////
// UPBEquipmentManagerComponent

UPBEquipmentManagerComponent::UPBEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UPBEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

UPBEquipmentInstance* UPBEquipmentManagerComponent::EquipItem(UPBEquipmentDefinition* EquipmentClass, UObject* InInstigator)
{
	UPBEquipmentInstance* Result = nullptr;

	if (EquipmentClass != nullptr)
	{
		Result = EquipmentList.AddEntry(EquipmentClass);
		if (Result != nullptr)
		{
			Result->SetInstigator(InInstigator);
			Result->OnEquipped();
			
			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	return Result;
}

void UPBEquipmentManagerComponent::UnequipItem(UPBEquipmentInstance* EquipmentInstance)
{
	if (EquipmentInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(EquipmentInstance);
		}

		EquipmentInstance->OnUnequipped();
		EquipmentList.RemoveEntry(EquipmentInstance);
	}
}

bool UPBEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FPBAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		UPBEquipmentInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UPBEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UPBEquipmentManagerComponent::UninitializeComponent()
{
	TArray<UPBEquipmentInstance*> AllEquipmentInstances;

	// gathering all instances before removal to avoid side effects affecting the equipment list iterator	
	for (const FPBAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (UPBEquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

void UPBEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing HEquipmentInstances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FPBAppliedEquipmentEntry& Entry : EquipmentList.Entries)
		{
			UPBEquipmentInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

UPBEquipmentInstance* UPBEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UPBEquipmentInstance> InstanceType)
{
	for (FPBAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UPBEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<UPBEquipmentInstance*> UPBEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UPBEquipmentInstance> InstanceType) const
{
	TArray<UPBEquipmentInstance*> Results;
	for (const FPBAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UPBEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}

