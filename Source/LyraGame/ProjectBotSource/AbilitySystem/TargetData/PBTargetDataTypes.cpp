// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/AbilitySystem/TargetData/PBTargetDataTypes.h"


bool FGameplayAbilityTargetData_PBMeleeInput::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	Ar << Combo;
	Ar << AttackAngle;

	bOutSuccess = true;
	return true;
}
