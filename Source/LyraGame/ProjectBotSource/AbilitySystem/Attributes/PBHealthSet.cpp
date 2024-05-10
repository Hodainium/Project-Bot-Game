// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/AbilitySystem/Attributes/PBHealthSet.h"

#include "Net/UnrealNetwork.h"

UPBHealthSet::UPBHealthSet()
	: HealthRegenRate(0.0f)
{
}

void UPBHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBHealthSet, HealthRegenRate, COND_None, REPNOTIFY_Always);
}

void UPBHealthSet::OnRep_HealthRegenRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPBHealthSet, HealthRegenRate, OldValue);
}

void UPBHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttribute(Attribute, NewValue);

	if (Attribute == GetHealthRegenRateAttribute())
	{
		// Do not allow health regen to drop below 0.
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}
