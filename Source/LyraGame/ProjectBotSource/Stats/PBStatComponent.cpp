// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Stats/PBStatComponent.h"
#include "ProjectBotSource/Inventory/PBInventoryComponent.h"
#include "AbilitySystemComponent.h"
#include "PBStatDefinition.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Character/LyraCharacter.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "ProjectBotSource/AbilitySystem/Attributes/PBStatsSet.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Stats_StatDef_Changed, "Stats.Message.StatDefChanged");

int FPBStatLevel::GetCost() const
{
	FPBStatLevelEntry Entry;

	if(StatDef->GetStatEntryForLevel(Level, Entry))
	{
		return Entry.Cost;
	}

	UE_LOGFMT(LogPBStats, Error, "StatLevel::GetCost(). StatDef {0} Does not have cost for level: {1}", StatDef->StatName.ToString(), Level);

	return 0;
}

bool FPBStatInstance::ContainsLevel(int LevelToCheck) const
{
	for (const FPBStatGrantedHandles& Handle : GrantedAbilitySetHandles)
	{
		if (Handle.StatLevel == LevelToCheck)
		{
			return true;
		}
	}

	UE_LOGFMT(LogPBStats, Error, "StatInstance::ContainsLevel(). StatDef {0} Does not contain level: {1}", StatDef->StatName.ToString(), LevelToCheck);

	return false;
}

int FPBStatInstance::GetHandlesIndexForLevel(int LevelToGet)
{
	for (int i = 0; i < GrantedAbilitySetHandles.Num(); i++)
	{
		if (GrantedAbilitySetHandles[i].StatLevel == LevelToGet)
		{
			return i;
		}
	}

	UE_LOGFMT(LogPBStats, Error, "StatInstance::GetHandles(). StatDef {0} Could not find Handles for level: {1}", StatDef->StatName.ToString(), LevelToGet);

	return -1;
}

UPBStatComponent::UPBStatComponent(const FObjectInitializer& ObjectInitializer)
{
	IsPendingServerConfirmation = false;
}

void UPBStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool UPBStatComponent::RequestStatPowerUp(UPBStatDefinition* StatDef)
{
	if(IsPowerRequestValid(StatDef, 1))
	{
		UE_LOGFMT(LogPBStats, Warning, "CLIENT: RequestPowerUp for StatDef: {0}. Pending Set to true", StatDef->StatName.ToString());

		IsPendingServerConfirmation = true;

		Server_TryPowerChange(StatDef, 1);

		return true;
	}

	return false;
}

bool UPBStatComponent::RequestStatPowerDown(UPBStatDefinition* StatDef)
{
	if (IsPowerRequestValid(StatDef, -1))
	{
		UE_LOGFMT(LogPBStats, Warning, "CLIENT: RequestPowerDown for StatDef: {0}. Pending Set to true", StatDef->StatName.ToString());

		IsPendingServerConfirmation = true;

		Server_TryPowerChange(StatDef, -1);

		return true;
	}

	return false;
}

void UPBStatComponent::Client_ConfirmCartCheckout_Implementation(bool bSuccessful)
{
	UE_LOGFMT(LogPBStats, Warning, "CLIENT: Cart Checkout was succesful: {0}. IsPending Reset", bSuccessful);
	IsPendingServerConfirmation = false;

	//Need to empty like this so it broadcasts
	EmptyCart();
}

void UPBStatComponent::GrantStatLevelPowerGE(UPBStatDefinition* StatDef) //TODO for this and max add active effect handle to stat level
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		UE_LOGFMT(LogPBStats, Warning, "Granted stat level GE for StatDef: {0}", StatDef->StatName.ToString());

		int StatIndex = GetStatInstanceIndex(StatDef);

		if (StatIndex < 0)
		{
			return;
		}

		UGameplayEffect* GE = StatDef->StatLevelGE->GetDefaultObject<UGameplayEffect>();

		StatInstances[StatIndex].GrantedStatGEs.Add(LinkedASC->ApplyGameplayEffectToSelf(GE, 1.f, LinkedASC->MakeEffectContext()));
	}
}

void UPBStatComponent::GrantStatMaxLevelPowerGE(UPBStatDefinition* StatDef)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		UE_LOGFMT(LogPBStats, Warning, "Granted max stat level GE for StatDef: {0}", StatDef->StatName.ToString());

		int StatIndex = GetStatInstanceIndex(StatDef);

		if (StatIndex < 0)
		{
			return;
		}

		UGameplayEffect* GE = StatDef->MaxStatLevelGE->GetDefaultObject<UGameplayEffect>();

		StatInstances[StatIndex].GrantedMaxStatGEs.Add(LinkedASC->ApplyGameplayEffectToSelf(GE, 1.f, LinkedASC->MakeEffectContext()));
	}
}

void UPBStatComponent::RemoveStatLevelPowerGE(UPBStatDefinition* StatDef)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		UE_LOGFMT(LogPBStats, Warning, "Removing stat level GE for StatDef: {0}", StatDef->StatName.ToString());

		int StatIndex = GetStatInstanceIndex(StatDef);

		if (StatIndex < 0)
		{
			return;
		}

		if(StatInstances[StatIndex].GrantedStatGEs.Num() > 0)
		{
			int LastIndex = StatInstances[StatIndex].GrantedStatGEs.Num() - 1;

			LinkedASC->RemoveActiveGameplayEffect(StatInstances[StatIndex].GrantedStatGEs[LastIndex], 1);

			StatInstances[StatIndex].GrantedStatGEs.RemoveAt(LastIndex);
		}
	}
}

void UPBStatComponent::RemoveStatMaxLevelPowerGE(UPBStatDefinition* StatDef)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		UE_LOGFMT(LogPBStats, Warning, "Removing max stat level GE for StatDef: {0}", StatDef->StatName.ToString());

		int StatIndex = GetStatInstanceIndex(StatDef);

		if (StatIndex < 0)
		{
			return;
		}

		if (StatInstances[StatIndex].GrantedMaxStatGEs.Num() > 0)
		{
			int LastIndex = StatInstances[StatIndex].GrantedMaxStatGEs.Num() - 1;

			LinkedASC->RemoveActiveGameplayEffect(StatInstances[StatIndex].GrantedMaxStatGEs[LastIndex], 1);

			StatInstances[StatIndex].GrantedMaxStatGEs.RemoveAt(LastIndex);
		}
	}
}

int UPBStatComponent::FindStatInstanceIndexForAttribute(FGameplayAttribute Attribute)
{
	for (int i = 0; i < StatInstances.Num(); i++)
	{
		if (StatInstances[i].StatDef->CurrentValueAttribute == Attribute || StatInstances[i].StatDef->MaxValueAttribute == Attribute)
		{
			return i;
		}
	}

	return -1;
}

void UPBStatComponent::HandleStatLevelAttributeChanged(const FOnAttributeChangeData& Data)
{
	//Loop through statdefs find and grant/remove abilitysets. This is the only place we should grant and remove abilitysets

	UE_LOGFMT(LogPBStats, Warning, "Handling stat level change for attribute {0}. Changed from {1} to {2}", Data.Attribute.GetName(), Data.OldValue, Data.NewValue);

	int StatIndex = FindStatInstanceIndexForAttribute(Data.Attribute);

	if(StatIndex < 0)
	{
		return;
	}

	if(Data.OldValue < Data.NewValue)
	{
		for (int i = Data.OldValue + 1; i < Data.NewValue + 1; i++) 
		{
			if (!StatInstances[StatIndex].ContainsLevel(i))
			{
				//Grant effect
				FPBStatGrantedHandles Handle = GrantStatAbilitySet(StatInstances[StatIndex].StatDef, i);
				Handle.StatLevel = i;
				StatInstances[StatIndex].GrantedAbilitySetHandles.Add(Handle);
			}
		}
	}
	else
	{
		for (int i = Data.NewValue + 1; i < Data.OldValue + 1; i++) 
		{
			UE_LOGFMT(LogPBStats, Warning, "TEMP: Removing abilityset for attribute {0} at level {1}", Data.Attribute.GetName(), i);

			//Remove effects
			if (StatInstances[StatIndex].ContainsLevel(i))
			{
				RemoveStatAbilitySet(StatIndex, i);
			}
		}
	}

	BroadcastStatAttributeChangeMessage(Data.Attribute);
}

void UPBStatComponent::HandleMaxStatLevelAttributeChanged(const FOnAttributeChangeData& Data)
{
	UE_LOGFMT(LogPBStats, Warning, "Handling max stat level change for attribute {0}. Changed from {1} to {2}", Data.Attribute.GetName(), Data.OldValue, Data.NewValue);

	BroadcastStatAttributeChangeMessage(Data.Attribute);
}

UPBInventoryComponent* UPBStatComponent::GetInventoryComponentFromOwner()
{
	ALyraCharacter* LyraChar = Cast<ALyraCharacter>(GetOwner());

	UPBInventoryComponent* InventoryComp = LyraChar->GetComponentByClass<UPBInventoryComponent>();

	return InventoryComp;
}

bool UPBStatComponent::IsPowerRequestValid(UPBStatDefinition* StatDef, int PowerChangeAmount)
{
	int NewLevel = GetStatLevelForStatDef(StatDef) + PowerChangeAmount;
	int CurrentMaxLevel = GetMaxStatLevelForStatDef(StatDef);

	bool bValidBounds = (NewLevel <= CurrentMaxLevel) && (NewLevel >= 0);

	bool bMeetsCost = GetPowerBankLevel() > 0 || PowerChangeAmount < 0 || StatDef == InitialStatsAsset->PowerBankStartingStats.StatDef;

	return bValidBounds && bMeetsCost;
}

bool UPBStatComponent::IsMaxPowerRequestValid(UPBStatDefinition* StatDef, int PowerChangeAmount)
{
	int NewMax = GetMaxStatLevelForStatDef(StatDef) + PowerChangeAmount;

	return NewMax <= StatDef->MaxLevel && NewMax >= 0;
}

bool UPBStatComponent::IsCartEntryValid(const FPBStatLevel& CartEntry)
{
	return CartEntry.StatDef->MaxLevel >= CartEntry.Level && CartEntry.Level != GetMaxStatLevelForStatDef(CartEntry.StatDef);
}

bool UPBStatComponent::TryAddToCart(FPBStatLevel CartEntry)
{
	if(CheckCartCost(CartEntry.GetCost()) && IsCartEntryValid(CartEntry) && !Cart.Contains(CartEntry))
	{
		Cart.Add(CartEntry);
		UE_LOGFMT(LogPBStats, Warning, "AddedToCart: Stat {0}, Level {1}", CartEntry.StatDef->StatName.ToString(), CartEntry.Level);
		BroadcastStatAttributeChangeMessage(CartEntry.StatDef);
		OnCartDataChanged.Broadcast();
		return true;
	}

	return false;
}

void UPBStatComponent::RemoveFromCart(FPBStatLevel CartEntry)
{
	int AmountRemoved = Cart.Remove(CartEntry);

	UE_LOGFMT(LogPBStats, Warning, "RemoveFromCart: Removed {0}", AmountRemoved);

	BroadcastStatAttributeChangeMessage(CartEntry.StatDef);
	OnCartDataChanged.Broadcast();

}

bool UPBStatComponent::RequestCartCheckout()
{
	if(!Cart.IsEmpty())
	{
		IsPendingServerConfirmation = true;

		Server_TryCartCheckout(Cart);

		return true;
	}

	return false;
}

void UPBStatComponent::EmptyCart()
{
	if (Cart.Num() > 0)
	{
		TArray<UPBStatDefinition*> StatDefs;

		for (const FPBStatLevel& StatLevel : Cart)
		{
			StatDefs.Add(StatLevel.StatDef);
		}

		Cart.Empty();

		for (UPBStatDefinition* StatDef : StatDefs)
		{
			BroadcastStatAttributeChangeMessage(StatDef);
		}
	}

	OnCartDataChanged.Broadcast();
}

bool UPBStatComponent::GetIsPendingServerConfirmation()
{
	return IsPendingServerConfirmation;
}

bool UPBStatComponent::GetHighestCartEntryForStatDef(UPBStatDefinition* StatDef, FPBStatLevel& CartEntry)
{
	bool bFound = false;

	for (const FPBStatLevel& Entry : Cart)
	{
		if (StatDef == Entry.StatDef)
		{
			if(CartEntry.Level <= Entry.Level)
			{
				CartEntry = Entry;
				bFound = true;
			}
		}
	}

	return bFound;
}

FPBStatLevel UPBStatComponent::CreateNextHighestCartEntryForStatDef(UPBStatDefinition* StatDef)
{
	FPBStatLevel OutEntry;

	OutEntry.StatDef = StatDef;
	OutEntry.Level = GetMaxStatLevelForStatDef(StatDef) + 1 + GetCartCountForStatDefinition(StatDef);

	return OutEntry;
}

void UPBStatComponent::Client_ConfirmPowerChange_Implementation(bool bSuccessful)
{
	UE_LOGFMT(LogPBStats, Warning, "CLIENT: PowerChange was succesful: {0}. IsPending Reset", bSuccessful);
	IsPendingServerConfirmation = false;
}

int UPBStatComponent::GetPowerBankLevel()
{
	return GetStatLevelForStatDef(InitialStatsAsset->PowerBankStartingStats.StatDef);
}

int UPBStatComponent::GetPowerBankMaxLevel()
{
	return GetMaxStatLevelForStatDef(InitialStatsAsset->PowerBankStartingStats.StatDef);
}

int UPBStatComponent::GetCartCountForStatDefinition(UPBStatDefinition* StatDef)
{
	int Count = 0;

	for (const FPBStatLevel& StatLevel : Cart)
	{
		if(StatLevel.StatDef == StatDef)
		{
			Count++;
		}
	}

	return Count;
}

void UPBStatComponent::InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC)
{
	UE_LOGFMT(LogPBStats, Warning, "Initting ASC. With name: {0}", InASC->GetName());

	LinkedASC = InASC;

	GrantInitialStatsAsset();

	for (FPBStatInstance& StatInstance : StatInstances)
	{
		StatInstance.StatLevelListener = InASC->GetGameplayAttributeValueChangeDelegate(StatInstance.StatDef->CurrentValueAttribute).AddUObject(this, &ThisClass::HandleStatLevelAttributeChanged);
		StatInstance.MaxStatLevelListener = InASC->GetGameplayAttributeValueChangeDelegate(StatInstance.StatDef->MaxValueAttribute).AddUObject(this, &ThisClass::HandleMaxStatLevelAttributeChanged);

		bool bFound = false;

		FOnAttributeChangeData StatLevelAttributeData;
		StatLevelAttributeData.OldValue = 0.f;
		StatLevelAttributeData.NewValue = InASC->GetGameplayAttributeValue(StatInstance.StatDef->CurrentValueAttribute, bFound);
		StatLevelAttributeData.Attribute = StatInstance.StatDef->CurrentValueAttribute;
		HandleStatLevelAttributeChanged(StatLevelAttributeData);

		FOnAttributeChangeData StatMaxLevelAttributeData;
		StatMaxLevelAttributeData.OldValue = 0.f;
		StatMaxLevelAttributeData.NewValue = InASC->GetGameplayAttributeValue(StatInstance.StatDef->MaxValueAttribute, bFound);
		StatMaxLevelAttributeData.Attribute = StatInstance.StatDef->MaxValueAttribute;
		HandleMaxStatLevelAttributeChanged(StatMaxLevelAttributeData);
	}
}

void UPBStatComponent::UninitializeFromAbilitySystem()
{
	UE_LOGFMT(LogPBStats, Warning, "Unitting ASC. TODO remove abilities here");

	RemoveAllStatInstances();

	LinkedASC = nullptr;
}

int UPBStatComponent::GetStatLevelForStatDef(UPBStatDefinition* StatDef)
{
	if(LinkedASC)
	{
		bool bFound;
		return LinkedASC->GetGameplayAttributeValue(StatDef->CurrentValueAttribute, bFound);
	}

	UE_LOGFMT(LogPBStats, Error, "GetStatLevel: ASC invalid");

	return 0;
}

int UPBStatComponent::GetMaxStatLevelForStatDef(UPBStatDefinition* StatDef)
{
	if (LinkedASC)
	{
		bool bFound;
		return LinkedASC->GetGameplayAttributeValue(StatDef->MaxValueAttribute, bFound);
	}

	UE_LOGFMT(LogPBStats, Error, "GetMaxStatLevel: ASC invalid");

	return 0;
}

void UPBStatComponent::Server_TryPowerChange_Implementation(UPBStatDefinition* StatDef, int PowerChangeAmount)
{
	bool bWasSuccessful = false;

	if(IsPowerRequestValid(StatDef, PowerChangeAmount))
	{
		if(PowerChangeAmount > 0)
		{
			for (int i = 0; i < PowerChangeAmount; i++)
			{
				GrantStatLevelPowerGE(StatDef);
			}
		}
		else
		{
			for (int i = PowerChangeAmount; i < 0; i++)
			{
				RemoveStatLevelPowerGE(StatDef);
			}
		}
		bWasSuccessful = true;
	}

	UE_LOGFMT(LogPBStats, Warning, "SERVER: Power change request was successful: {0}", bWasSuccessful);

	Client_ConfirmPowerChange(bWasSuccessful);
}

void UPBStatComponent::Server_TryCartCheckout_Implementation(const TArray<FPBStatLevel>& InCart)
{
	Cart = InCart;
	bool bWasSuccessful = false;

	if(CheckCartCost())
	{
		SpendCartCost(GetCartCost());

		for (const FPBStatLevel& StatLevel : InCart)
		{
			if(IsCartEntryValid(StatLevel))
			{
				if (StatLevel.StatDef == InitialStatsAsset->PowerBankStartingStats.StatDef)
				{
					GrantStatLevelPowerGE(StatLevel.StatDef);
				}

				GrantStatMaxLevelPowerGE(StatLevel.StatDef);
				bWasSuccessful = true;
			}
		}
	}

	UE_LOGFMT(LogPBStats, Warning, "SERVER: Checking out Cart was successful: {0}", bWasSuccessful);

	//Need to call first so that listen server can broadcast changes
	Client_ConfirmCartCheckout(bWasSuccessful);

	Cart.Empty();
}

bool UPBStatComponent::CheckCartCost(int CostToAdd)
{
	int TotalCost = GetCartCost() + CostToAdd;

	if(UPBInventoryComponent* InventoryComp = GetInventoryComponentFromOwner())
	{
		if(InventoryComp->GetTotalItemCountByDefinition(InitialStatsAsset->CostItem) >= TotalCost)
		{
			return true;
		}

		UE_LOGFMT(LogPBStats, Error, "CheckCartCost. Not enough money! {0}", InventoryComp->GetTotalItemCountByDefinition(InitialStatsAsset->CostItem));
	}

	UE_LOGFMT(LogPBStats, Error, "CheckCartCost. InvComp Invalid!");


	return false;
}

bool UPBStatComponent::SpendCartCost(int CostToSpend)
{
	if (UPBInventoryComponent* InventoryComp = GetInventoryComponentFromOwner())
	{
		InventoryComp->ConsumeItemsByDefinition(InitialStatsAsset->CostItem, CostToSpend);
		return true;
	}

	return false;
}

int UPBStatComponent::GetCartCost()
{
	int Total = 0;

	FPBStatLevelEntry Entry;

	for (const FPBStatLevel& StatLevel : Cart)
	{
		//This should never be an invalid access because it's already checked but we can validate it anyway
		if(StatLevel.StatDef->GetStatEntryForLevel(StatLevel.Level, Entry))
		{
			Total += Entry.Cost;
		}
	}

	return Total;
}

int UPBStatComponent::GetStatInstanceIndex(UPBStatDefinition* StatDef)
{
	for (int i = 0; i < StatInstances.Num(); i++)
	{
		if (StatInstances[i].StatDef == StatDef)
		{
			return i;
		}
	}

	UE_LOGFMT(LogPBStats, Error, "Could not find StatInstance for StatDef: {0}", StatDef->StatName.ToString());

	return -1;
}

FPBStatGrantedHandles UPBStatComponent::GrantStatAbilitySet(UPBStatDefinition* StatDef, int StatLevel) //TODO
{
	UE_LOGFMT(LogPBStats, Warning, "Granting ability set for stat def: {0} at level {1}", StatDef->StatName.ToString(), StatLevel);

	FPBStatGrantedHandles OutHandles;

	ALyraCharacter* LyraChar = Cast<ALyraCharacter>(GetOwner());

	ULyraAbilitySystemComponent* LyraASC = LyraChar->GetLyraAbilitySystemComponent();

	if(LyraASC)
	{
		FPBStatLevelEntry Entry;

		if (StatDef->GetStatEntryForLevel(StatLevel, Entry))
		{
			for (auto AbilitySet : Entry.AbilitySetsToGrant)
			{
				AbilitySet->GiveToAbilitySystem(LyraASC, &OutHandles.GrantedAbilitySetHandles);
			}
		}
	}

	return OutHandles;
}

void UPBStatComponent::RemoveStatAbilitySet(int InstanceIndex, int StatLevel)
{
	UE_LOGFMT(LogPBStats, Warning, "Removing ability set for stat instance: {0} at level {1}", StatInstances[InstanceIndex].StatDef->StatName.ToString(), StatLevel);


	if(InstanceIndex < 0)
	{
		return;
	}

	ALyraCharacter* LyraChar = Cast<ALyraCharacter>(GetOwner());

	ULyraAbilitySystemComponent* LyraASC = LyraChar->GetLyraAbilitySystemComponent();

	if(!LyraASC)
	{
		return;
	}

	int HandlesIndex = StatInstances[InstanceIndex].GetHandlesIndexForLevel(StatLevel);

	if(HandlesIndex < 0)
	{
		return;
	}

	StatInstances[InstanceIndex].GrantedAbilitySetHandles[HandlesIndex].GrantedAbilitySetHandles.TakeFromAbilitySystem(LyraASC);
	StatInstances[InstanceIndex].GrantedAbilitySetHandles.RemoveAt(HandlesIndex);
}

void UPBStatComponent::GrantInitialStatsAsset()
{
	UE_LOGFMT(LogPBStats, Warning, "Granting initial stats");

	GrantInitialStat(InitialStatsAsset->PowerBankStartingStats);

	for (const FPBInitialStat& InitialStat : InitialStatsAsset->StartingStatLevels)
	{
		GrantInitialStat(InitialStat);
	}
}

void UPBStatComponent::RemoveAllStatInstances()
{
	UE_LOGFMT(LogPBStats, Warning, "Removing all stat instances");

	for (FPBStatInstance& StatInstance : StatInstances)
	{
		StatInstance.StatLevelListener.Reset();
		StatInstance.MaxStatLevelListener.Reset();

		if (GetOwnerRole() == ROLE_Authority)
		{
			RemoveStatInstance(StatInstance);
		}
	}

	StatInstances.Empty();
}

void UPBStatComponent::GrantInitialStat(const FPBInitialStat& InInitialStat)
{
	UE_LOGFMT(LogPBStats, Warning, "Granting initial stat: {0} at level {1} with max {2}", InInitialStat.StatDef->StatName.ToString(), InInitialStat.InitialStatLevel, InInitialStat.InitialMaxStatLevel);

	StatInstances.Add(FPBStatInstance(InInitialStat.StatDef));

	if (GetOwnerRole() == ROLE_Authority)
	{
		for (int i = 0; i < InInitialStat.InitialMaxStatLevel; i++)
		{
			GrantStatMaxLevelPowerGE(InInitialStat.StatDef);
		}

		for (int i = 0; i < InInitialStat.InitialStatLevel; i++)
		{
			if (IsPowerRequestValid(InInitialStat.StatDef, 1))
			{
				GrantStatLevelPowerGE(InInitialStat.StatDef);
			}
		}
	}
}

void UPBStatComponent::RemoveStatInstance(FPBStatInstance& InStatInstance)
{
	UE_LOGFMT(LogPBStats, Warning, "Removing initial stat: {0}", InStatInstance.StatDef->StatName.ToString());

	for (FPBStatGrantedHandles& StatHandle : InStatInstance.GrantedAbilitySetHandles)
	{
		StatHandle.GrantedAbilitySetHandles.TakeFromAbilitySystem(LinkedASC);
	}
	InStatInstance.GrantedAbilitySetHandles.Empty();

	for (const FActiveGameplayEffectHandle& GEHandle : InStatInstance.GrantedStatGEs)
	{
		LinkedASC->RemoveActiveGameplayEffect(GEHandle, 1);
	}
	InStatInstance.GrantedStatGEs.Empty();

	for (const FActiveGameplayEffectHandle& GEHandle : InStatInstance.GrantedMaxStatGEs)
	{
		LinkedASC->RemoveActiveGameplayEffect(GEHandle, 1);
	}
	InStatInstance.GrantedMaxStatGEs.Empty();
}

UPBStatDefinition* UPBStatComponent::GetStatDefinitionForAttribute(const FGameplayAttribute& InAttribute) const
{
	if(InitialStatsAsset->PowerBankStartingStats.StatDef->CurrentValueAttribute == InAttribute || 
		InitialStatsAsset->PowerBankStartingStats.StatDef->MaxValueAttribute == InAttribute)
	{
		return InitialStatsAsset->PowerBankStartingStats.StatDef;
	}

	for (const FPBInitialStat& Stat : InitialStatsAsset->StartingStatLevels)
	{
		if (Stat.StatDef->CurrentValueAttribute == InAttribute ||
			Stat.StatDef->MaxValueAttribute == InAttribute)
		{
			return Stat.StatDef;
		}
	}

	UE_LOGFMT(LogPBStats, Error, "Could not find StatDef for Attribute: {0}", InAttribute.GetName());

	return nullptr;
}

void UPBStatComponent::BroadcastStatAttributeChangeMessage(const FGameplayAttribute& InAttribute) const
{
	BroadcastStatAttributeChangeMessage(GetStatDefinitionForAttribute(InAttribute));
}

void UPBStatComponent::BroadcastStatAttributeChangeMessage(UPBStatDefinition* InStatDef) const
{
	UE_LOGFMT(LogPBStats, Warning, "Broadcasting change for statdef: {0}", InStatDef->StatName.ToString());

	FPBStatDefChangedMessage Message;
	Message.StatComponent = this;
	Message.StatDef = InStatDef;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(TAG_Stats_StatDef_Changed, Message);
}
