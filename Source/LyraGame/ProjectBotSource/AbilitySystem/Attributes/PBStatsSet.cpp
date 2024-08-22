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

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxStrength, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, PowerBank, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxPowerBank, COND_None, REPNOTIFY_Always);

	///////////////////////////////////////////////////

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxDexterity, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Marksmanship, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxMarksmanship, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Radiance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxRadiance, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Strike, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxStrike, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Pierce, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxPierce, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Slash, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxSlash, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Fire, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxFire, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Ice, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxIce, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Lightning, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxLightning, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Decay, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxDecay, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxArmor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxEnergy, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Repair, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxRepair, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Scan, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxScan, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxShield, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Grab, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxGrab, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Charge, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxCharge, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Special, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxSpecial, COND_None, REPNOTIFY_Always);


}

void UPBStatsSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Strength, OldValue);
}

void UPBStatsSet::OnRep_MaxStrength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxStrength, OldValue);
}

void UPBStatsSet::OnRep_Dexterity(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Dexterity, OldValue);
}

void UPBStatsSet::OnRep_MaxDexterity(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxDexterity, OldValue);
}

void UPBStatsSet::OnRep_Marksmanship(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Marksmanship, OldValue);
}

void UPBStatsSet::OnRep_MaxMarksmanship(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxMarksmanship, OldValue);
}

void UPBStatsSet::OnRep_Radiance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Radiance, OldValue);
}

void UPBStatsSet::OnRep_MaxRadiance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxRadiance, OldValue);
}

void UPBStatsSet::OnRep_Strike(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Strike, OldValue);
}

void UPBStatsSet::OnRep_MaxStrike(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxStrike, OldValue);
}

void UPBStatsSet::OnRep_Pierce(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Pierce, OldValue);
}

void UPBStatsSet::OnRep_MaxPierce(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxPierce, OldValue);
}

void UPBStatsSet::OnRep_Slash(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Slash, OldValue);
}

void UPBStatsSet::OnRep_MaxSlash(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxSlash, OldValue);
}

void UPBStatsSet::OnRep_Fire(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Fire, OldValue);
}

void UPBStatsSet::OnRep_MaxFire(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxFire, OldValue);
}

void UPBStatsSet::OnRep_Ice(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Ice, OldValue);
}

void UPBStatsSet::OnRep_MaxIce(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxIce, OldValue);
}

void UPBStatsSet::OnRep_Lightning(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Lightning, OldValue);
}

void UPBStatsSet::OnRep_MaxLightning(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxLightning, OldValue);
}

void UPBStatsSet::OnRep_Decay(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Decay, OldValue);
}

void UPBStatsSet::OnRep_MaxDecay(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxDecay, OldValue);
}

void UPBStatsSet::OnRep_Armor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Armor, OldValue);
}

void UPBStatsSet::OnRep_MaxArmor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxArmor, OldValue);
}

void UPBStatsSet::OnRep_Energy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Energy, OldValue);
}

void UPBStatsSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxEnergy, OldValue);
}

void UPBStatsSet::OnRep_Repair(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Repair, OldValue);
}

void UPBStatsSet::OnRep_MaxRepair(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxRepair, OldValue);
}

void UPBStatsSet::OnRep_Scan(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Scan, OldValue);
}

void UPBStatsSet::OnRep_MaxScan(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxScan, OldValue);
}

void UPBStatsSet::OnRep_Shield(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Shield, OldValue);
}

void UPBStatsSet::OnRep_MaxShield(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxShield, OldValue);
}

void UPBStatsSet::OnRep_Grab(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Grab, OldValue);
}

void UPBStatsSet::OnRep_MaxGrab(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxGrab, OldValue);
}

void UPBStatsSet::OnRep_Charge(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Charge, OldValue);
}

void UPBStatsSet::OnRep_MaxCharge(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxCharge, OldValue);
}

void UPBStatsSet::OnRep_Special(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, Special, OldValue);
}

void UPBStatsSet::OnRep_MaxSpecial(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxSpecial, OldValue);
}

void UPBStatsSet::OnRep_PowerBank(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, PowerBank, OldValue);
}

void UPBStatsSet::OnRep_MaxPowerBank(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBStatsSet, MaxPowerBank, OldValue);
}
