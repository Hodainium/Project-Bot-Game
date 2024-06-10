// Copyright Epic Games, Inc. All Rights Reserved.

#include "PBKnockbackExecution.h"

#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "AbilitySystem/Attributes/LyraCombatSet.h"
#include "Logging/StructuredLog.h"
#include "ProjectBotSource/AbilitySystem/Attributes/PBCombatSet.h"
#include "ProjectBotSource/AbilitySystem/Attributes/PBHealthSet.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PBKnockbackExecution)


struct FKnockbackStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseKnockbackDef;

	FKnockbackStatics()
	{
		BaseKnockbackDef = FGameplayEffectAttributeCaptureDefinition(UPBCombatSet::GetBaseKnockbackAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FKnockbackStatics& KnockbackStatics()
{
	static FKnockbackStatics Statics;
	return Statics;
}


UPBKnockbackExecution::UPBKnockbackExecution()
{
	RelevantAttributesToCapture.Add(KnockbackStatics().BaseKnockbackDef);
}

void UPBKnockbackExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE

	//Here we can change the hit result angle depending on what tags the knockback has on it

	//Example:
	//Radial from actor
	//


	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	//bool bIsMelee = true;

	//if(bIsMelee)
	//{
	//	
	//}

	float BaseKnockback = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(KnockbackStatics().BaseKnockbackDef, EvaluateParameters, BaseKnockback);

	const float KnockbackDone = FMath::Max(0.0f, BaseKnockback);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UPBHealthSet::GetKnockbackAttribute(), EGameplayModOp::Additive, KnockbackDone));
#endif // #if WITH_SERVER_CODE
}

