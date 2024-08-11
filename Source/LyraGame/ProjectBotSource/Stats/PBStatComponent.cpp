// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Stats/PBStatComponent.h"
#include "ProjectBotSource/Inventory/PBInventoryComponent.h"
#include "AbilitySystemComponent.h"
#include "PBStatDefinition.h"
#include "Character/LyraCharacter.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"
#include "ProjectBotSource/AbilitySystem/Attributes/PBStatsSet.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"


int FPBStatLevel::GetCost() const
{
	if(StatDef->StatEntries.IsValidIndex(Level))
	{
		return StatDef->StatEntries[Level].Cost;
	}

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

	return -1;
}

UPBStatComponent::UPBStatComponent(const FObjectInitializer& ObjectInitializer)
{
	IsPendingServerConfirmation = false;
}

void UPBStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StatInstances);
}

void UPBStatComponent::RequestStatPowerUp(UPBStatDefinition* StatDef)
{
	IsPendingServerConfirmation = true;

	Server_TryPowerChange(StatDef, 1);
}

void UPBStatComponent::RequestStatPowerDown(UPBStatDefinition* StatDef)
{
	IsPendingServerConfirmation = true;

	Server_TryPowerChange(StatDef, -1);
}

void UPBStatComponent::Client_ConfirmCartCheckout_Implementation(bool bSuccessful)
{
	UE_LOGFMT(LogPBGame, Warning, "pending set to false");
	IsPendingServerConfirmation = false;
	Cart.Empty();

	//if (!bSuccessful)
	//{
	//	PendingSlotData.Reset();
	//}

	//if (PendingSlotData.IsEmpty())
	//{
	//	//Broadcast a message here
	//	if (OnReceivedServerSwapData.IsBound())
	//	{
	//		UE_LOGFMT(LogPBGame, Warning, "broadcasting data recieved from client rpc");
	//		OnReceivedServerSwapData.Broadcast();
	//	}
	//}


	UE_LOGFMT(LogPBGame, Warning, "pending tried to broadcast");
}

void UPBStatComponent::GrantStatLevelPowerGE(UPBStatDefinition* StatDef) //TODO for this and max add active effect handle to stat level
{
	if (GetOwnerRole() == ROLE_Authority)
	{
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
		int StatIndex = GetStatInstanceIndex(StatDef);

		if (StatIndex < 0)
		{
			return;
		}

		if(StatInstances[StatIndex].GrantedStatGEs.Num() > 0)
		{
			int LastIndex = StatInstances[StatIndex].GrantedStatGEs.Num() - 1;

			LinkedASC->RemoveActiveGameplayEffect(StatInstances[StatIndex].GrantedStatGEs[LastIndex]);

			StatInstances[StatIndex].GrantedStatGEs.RemoveAt(LastIndex);
		}
	}
}

void UPBStatComponent::RemoveStatMaxLevelPowerGE(UPBStatDefinition* StatDef)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		int StatIndex = GetStatInstanceIndex(StatDef);

		if (StatIndex < 0)
		{
			return;
		}

		if (StatInstances[StatIndex].GrantedMaxStatGEs.Num() > 0)
		{
			int LastIndex = StatInstances[StatIndex].GrantedMaxStatGEs.Num() - 1;

			LinkedASC->RemoveActiveGameplayEffect(StatInstances[StatIndex].GrantedMaxStatGEs[LastIndex]);

			StatInstances[StatIndex].GrantedMaxStatGEs.RemoveAt(LastIndex);
		}
	}
}

void UPBStatComponent::LinkASC(UAbilitySystemComponent* InASC)
{
	LinkedASC = InASC;
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

void UPBStatComponent::UnLinkASC()
{
	LinkedASC = nullptr;
	for (FPBStatInstance& StatInstance : StatInstances)
	{
		StatInstance.StatLevelListener.Reset();
		StatInstance.MaxStatLevelListener.Reset();
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

	int StatIndex = FindStatInstanceIndexForAttribute(Data.Attribute);

	if(StatIndex < 0)
	{
		return;
	}

	if(Data.OldValue < Data.NewValue)
	{
		for (int i = Data.OldValue; i < Data.NewValue; i++)
		{
			if (!StatInstances[StatIndex].ContainsLevel(i))
			{
				//Grant effect
				FPBStatGrantedHandles Handle = GrantStatAbilitySet(StatInstances[StatIndex].StatDef, i);
				StatInstances[StatIndex].GrantedAbilitySetHandles.Add(Handle);
			}
		}
	}
	else
	{
		for (int i = Data.NewValue; i < Data.OldValue; i++)
		{
			//Remove effects
			if (StatInstances[StatIndex].ContainsLevel(i))
			{
				RemoveStatAbilitySet(StatIndex, i);
			}
		}
	}
}

void UPBStatComponent::HandleMaxStatLevelAttributeChanged(const FOnAttributeChangeData& Data)
{
	//Loop through statdefs find and remove abilitysets. This is the only place we should grant and remove abilitysets

	unimplemented();
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

	bool bMeetsCost = GetPowerBankLevel() > 0;

	return bValidBounds && bMeetsCost;
}

bool UPBStatComponent::TryAddToCart(FPBStatLevel CartEntry)
{
	if(CheckCartCost(CartEntry.GetCost()))
	{
		Cart.Add(CartEntry);
		return true;
	}

	return false;
}

void UPBStatComponent::RemoveFromCart(FPBStatLevel CartEntry)
{
	Cart.Remove(CartEntry);
}

void UPBStatComponent::RequestCartCheckout()
{
	IsPendingServerConfirmation = true;

	Server_TryCartCheckout(Cart);
}

bool UPBStatComponent::GetIsPendingServerConfirmation()
{
	return IsPendingServerConfirmation;
}

void UPBStatComponent::Client_ConfirmPowerChange_Implementation(bool bSuccessful)
{
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

int UPBStatComponent::GetStatLevelForStatDef(UPBStatDefinition* StatDef)
{
	if(LinkedASC)
	{
		bool bFound;
		return LinkedASC->GetGameplayAttributeValue(StatDef->CurrentValueAttribute, bFound);
	}

	return 0;
}

int UPBStatComponent::GetMaxStatLevelForStatDef(UPBStatDefinition* StatDef)
{
	if (LinkedASC)
	{
		bool bFound;
		return LinkedASC->GetGameplayAttributeValue(StatDef->CurrentValueAttribute, bFound);
	}

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

	Client_ConfirmPowerChange(bWasSuccessful);
}

void UPBStatComponent::Server_TryCartCheckout_Implementation(const TArray<FPBStatLevel>& InCart)
{
	Cart = InCart;
	bool bWasSuccessful = false;

	if(CheckCartCost())
	{
		bWasSuccessful = true;

		for (const FPBStatLevel& StatLevel : InCart)
		{
			GrantStatMaxLevelPowerGE(StatLevel.StatDef);
		}
	}

	Cart.Empty();

	Client_ConfirmCartCheckout(bWasSuccessful);
}

bool UPBStatComponent::CheckCartCost(int CostToAdd)
{
	int TotalCost = GetCartCost() + CostToAdd;

	return true;
}

int UPBStatComponent::GetCartCost()
{
	int Total = 0;

	for (const FPBStatLevel& StatLevel : Cart)
	{
		//This should never be an invalid access because it's already checked but we can validate it anyway
		if(StatLevel.StatDef->StatEntries.IsValidIndex(StatLevel.Level))
		{
			Total += StatLevel.StatDef->StatEntries[StatLevel.Level].Cost;
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

	return -1;
}

FPBStatGrantedHandles UPBStatComponent::GrantStatAbilitySet(UPBStatDefinition* StatDef, int StatLevel) //TODO
{
	FPBStatGrantedHandles OutHandles;

	ALyraCharacter* LyraChar = Cast<ALyraCharacter>(GetOwner());

	ULyraAbilitySystemComponent* LyraASC = LyraChar->GetLyraAbilitySystemComponent();

	if(LyraASC)
	{
		if (StatDef->StatEntries.IsValidIndex(StatLevel))
		{
			for (auto AbilitySet : StatDef->StatEntries[StatLevel].AbilitySetsToGrant)
			{
				AbilitySet->GiveToAbilitySystem(LyraASC, &OutHandles.GrantedAbilitySetHandles);
			}
		}
	}

	return OutHandles;
}

void UPBStatComponent::RemoveStatAbilitySet(int InstanceIndex, int StatLevel) //TODO
{
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
	if (GetOwnerRole() == ROLE_Authority)
	{
		GrantInitialStat(InitialStatsAsset->PowerBankStartingStats);

		for (const FPBInitialStat& InitialStat : InitialStatsAsset->StartingStatLevels)
		{
			GrantInitialStat(InitialStat);
		}
	}
}

void UPBStatComponent::GrantInitialStat(const FPBInitialStat& InInitialStat)
{
	StatInstances.Add(FPBStatInstance(InInitialStat.StatDef));

	for (int i = 0; i < InInitialStat.InitialMaxStatLevel; i++)
	{
		GrantStatMaxLevelPowerGE(InInitialStat.StatDef);
	}

	for (int i = 0; i < InInitialStat.InitialStatLevel; i++)
	{
		GrantStatLevelPowerGE(InInitialStat.StatDef);
	}
}
