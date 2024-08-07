// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/AbilitySystem/Attributes/PBStatsSet.h"

#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"
#include "ProjectBotSource/Tags/PB_Tags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PBStatsSet)

UPBStatsSet::UPBStatsSet() : Strength(1.0f), MaxStrength(3.0f)
{
}

void UPBStatsSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, Strength, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPBStatsSet, MaxStrength, COND_OwnerOnly, REPNOTIFY_Always);
}

void UPBStatsSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
}

void UPBStatsSet::OnRep_MaxStrength(const FGameplayAttributeData& OldValue)
{
}
