// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/AbilitySystem/Attributes/PBStatsSet.h"

#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"
#include "ProjectBotSource/Tags/PB_Tags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PBStatsSet)

UPBStatsSet::UPBStatsSet() : PowerBank(0.f), MaxPowerBank(0.f), Strength(0.f), MaxStrength(0.f)
{
}

void UPBStatsSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Strength, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxStrength, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, PowerBank, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxPowerBank, COND_OwnerOnly, REPNOTIFY_Always);

	///////////////////////////////////////////////////

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Dexterity, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxDexterity, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Marksmanship, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxMarksmanship, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Radiance, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxRadiance, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Strike, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxStrike, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Pierce, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxPierce, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Slash, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxSlash, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Fire, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxFire, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Ice, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxIce, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Lightning, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxLightning, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Decay, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxDecay, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Armor, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxArmor, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Energy, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxEnergy, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Repair, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxRepair, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Scan, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxScan, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Shield, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxShield, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Grab, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxGrab, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Charge, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxCharge, COND_OwnerOnly, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Special, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxSpecial, COND_OwnerOnly, REPNOTIFY_Always);


}

void UPBStatsSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxStrength(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Dexterity(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxDexterity(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Marksmanship(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxMarksmanship(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Radiance(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxRadiance(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Strike(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxStrike(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Pierce(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxPierce(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Slash(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxSlash(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Fire(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxFire(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Ice(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxIce(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Lightning(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxLightning(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Decay(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxDecay(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Armor(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxArmor(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Energy(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Repair(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxRepair(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Scan(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxScan(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Shield(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxShield(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Grab(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxGrab(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Charge(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxCharge(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_Special(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxSpecial(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_PowerBank(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxPowerBank(const FGameplayAttributeData& OldValue)
{
}
