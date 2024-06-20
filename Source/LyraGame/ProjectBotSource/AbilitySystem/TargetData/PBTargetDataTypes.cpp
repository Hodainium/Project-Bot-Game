// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/AbilitySystem/TargetData/PBTargetDataTypes.h"


bool FGameplayAbilityTargetData_PBComboInput::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	Ar << Combo;

	bOutSuccess = true;
	return true;
}

bool FPBGameplayAbilityTargetData_MeleeHit::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayAbilityTargetData_SingleTargetHit::NetSerialize(Ar, Map, bOutSuccess);
	Ar << Combo;
	return true;
}
