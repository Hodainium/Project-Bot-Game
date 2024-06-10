// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/AbilitySystem/Attributes/PBCombatSet.h"

#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PBCombatSet)


UPBCombatSet::UPBCombatSet() : BaseKnockback(0.0f)
{
}

void UPBCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBCombatSet, BaseKnockback, COND_OwnerOnly, REPNOTIFY_Always);
}

void UPBCombatSet::OnRep_BaseKnockback(const FGameplayAttributeData& OldValue)
{
}
