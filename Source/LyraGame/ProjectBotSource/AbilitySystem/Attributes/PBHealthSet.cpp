// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/AbilitySystem/Attributes/PBHealthSet.h"

#include "GameplayEffectExtension.h"
#include "AbilitySystem/LyraGameplayAbilityTargetData_SingleTargetHit.h"
#include "AbilitySystem/LyraGameplayEffectContext.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"
#include "ProjectBotSource/Tags/PB_Tags.h"

UPBHealthSet::UPBHealthSet()
	: HealthRegenRate(0.0f)
{
}

void UPBHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPBHealthSet, HealthRegenRate, COND_None, REPNOTIFY_Always);
}

void UPBHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetKnockbackAttribute())
	{
		FGameplayEventData Payload;

		FLyraGameplayEffectContext* TypedContext = FLyraGameplayEffectContext::ExtractEffectContext(Data.EffectSpec.GetContext());
		check(TypedContext);

		FGameplayAbilityTargetDataHandle TargetDataHandle;

		FGameplayAbilityTargetData_LocationInfo* LocationInfo = new FGameplayAbilityTargetData_LocationInfo();

		/*FGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FGameplayAbilityTargetData_SingleTargetHit();

		if(FHitResult* HitResult = TypedContext->GetHitResult())
		{
			NewTargetData->HitResult = *HitResult;
		}*/

		FGameplayTagContainer AssetTags;

		Data.EffectSpec.GetAllAssetTags(AssetTags);

		float KnockbackLevel = 0.f;

		if(AssetTags.HasTagExact(PB_Knockback_Tags::TAG_KNOCKBACK_LEVEL_0))
		{
			KnockbackLevel = 0.f;
		}
		else if(AssetTags.HasTagExact(PB_Knockback_Tags::TAG_KNOCKBACK_LEVEL_1))
		{
			KnockbackLevel = 1.f;
		}
		else if (AssetTags.HasTagExact(PB_Knockback_Tags::TAG_KNOCKBACK_LEVEL_2))
		{
			KnockbackLevel = 2.f;
		}
		else if (AssetTags.HasTagExact(PB_Knockback_Tags::TAG_KNOCKBACK_LEVEL_3))
		{
			KnockbackLevel = 3.f;
		}
		else if (AssetTags.HasTagExact(PB_Knockback_Tags::TAG_KNOCKBACK_LEVEL_4))
		{
			KnockbackLevel = 4.f;
		}
		else if (AssetTags.HasTagExact(PB_Knockback_Tags::TAG_KNOCKBACK_LEVEL_5))
		{
			KnockbackLevel = 5.f;
		}

		FVector InstigatorLocation = TypedContext->GetEffectCauser()->GetActorLocation();
		FVector TargetLocation = Data.Target.GetAvatarActor()->GetActorLocation();

		// make 70 Data.EvaluatedData.Magnitude 
		FVector KnockbackDestination = TargetLocation + (TargetLocation - InstigatorLocation).GetSafeNormal2D() * Data.EvaluatedData.Magnitude; //70

		LocationInfo->TargetLocation.LiteralTransform.SetTranslation(KnockbackDestination);

		//TargetDataHandle.Add(NewTargetData);
		TargetDataHandle.Add(LocationInfo);


		Payload.EventTag = PB_GameplayEvent_Tags::TAG_GAMEPLAYEVENT_KNOCKBACK;
		Payload.Instigator = TypedContext->GetEffectCauser();  //GetInstigator();
		Payload.EventMagnitude = KnockbackLevel;
		//Payload.Target = Data.Target.GetAvatarActor();
		//Payload.OptionalObject = Data.EffectSpec.Def;
		//Payload.ContextHandle = Data.EffectSpec.GetEffectContext();

		//Payload.InstigatorTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
		//Payload.TargetTags = *Data.EffectSpec.CapturedTargetTags.GetAggregatedTags();

		//Payload.EventMagnitude = Data.EvaluatedData.Magnitude; //Should be used to pick which attack level
		Payload.TargetData = TargetDataHandle;

		//FScopedPredictionWindow NewScopedWindow(GetOwningAbilitySystemComponent(), true);
		GetOwningAbilitySystemComponent()->HandleGameplayEvent(Payload.EventTag, &Payload);

		UE_LOGFMT(LogPBGame, Warning, "Knockback in att set= {0}", Data.EvaluatedData.Magnitude);

		SetKnockback(0.0f);
	}
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
