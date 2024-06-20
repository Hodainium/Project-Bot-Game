// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/AbilitySystem/TargetData/BPFL_PBTargetTypes.h"

#include "PBTargetDataTypes.h"
#include "AbilitySystem/LyraGameplayAbilityTargetData_SingleTargetHit.h"
#include "Logging/StructuredLog.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"


int UBPFL_PBTargetTypes::GetComboNumFromTargetHandle(const FGameplayAbilityTargetDataHandle& Handle, const int Index)
{
	// NOTE, there is two versions of this '::Get(int32 Index)' function; 
	// 1) const version that returns 'const FGameplayAbilityTargetData*', good for reading target data values 
	// 2) non-const version that returns 'FGameplayAbilityTargetData*', good for modifying target data values
	const FGameplayAbilityTargetData* Data = Handle.Get(Index);
	// This will valid check the index for you

	UE_LOGFMT(LogPBGame, Warning, "TG: Checking idx: {0}", Index);


	// Valid check we have something to use, null data means nothing to cast for
	if (Data == nullptr)
	{
		UE_LOGFMT(LogPBGame, Warning, "TG: Returning -1 cause nullptr");
		return -1;
	}
	// This is basically the type checking pass, static_cast does not have type safety, this is why we do this check.
	// If we don't do this then it will object slice the struct and thus we have no way of making sure its that type.
	if (Data->GetScriptStruct() == FGameplayAbilityTargetData_PBComboInput::StaticStruct())
	{
		// Here is when you would do the cast because we know its the correct type already
		const FGameplayAbilityTargetData_PBComboInput* CustomData = static_cast<const FGameplayAbilityTargetData_PBComboInput*>(Data);
		return CustomData->Combo;
	}
	if (Data->GetScriptStruct() == FPBGameplayAbilityTargetData_MeleeHit::StaticStruct())
	{
		// Here is when you would do the cast because we know its the correct type already
		const FPBGameplayAbilityTargetData_MeleeHit* CustomData = static_cast<const FPBGameplayAbilityTargetData_MeleeHit*>(Data);
		return CustomData->Combo;
	}
	UE_LOGFMT(LogPBGame, Warning, "TG: Returning -1 cause didnt match");
	return -1;
}
