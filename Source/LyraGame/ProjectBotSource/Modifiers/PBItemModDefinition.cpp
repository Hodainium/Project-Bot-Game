// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Modifiers/PBItemModDefinition.h"

#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"
#include "ProjectBotSource/System/PBAssetManager.h"
#include "Logging/StructuredLog.h"
#include "ProjectBotSource/Weapons/PBModifiedWeaponInstance.h"


void FPBItemModDef_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FPBItemModDef_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	UE_LOGFMT(LogPBGame, Warning, "Trying to store GE handle");
	if (Handle.IsValid())
	{
		UE_LOGFMT(LogPBGame, Warning, "Did store GE handle");
		GameplayEffectHandles.Add(Handle);
	}
}

void FPBItemModDef_GrantedHandles::AddGameplayCue(const FGameplayEffectCue& InCue)
{
	GrantedPersistingCues.AppendTags(InCue.GameplayCueTags);
}

void FPBItemModDef_GrantedHandles::AddGrantedDamageGE(const FPBItemModDef_DamageType& Effect)
{
	GrantedDamageTypes.Add(Effect);
}

void FPBItemModDef_GrantedHandles::AddGrantedOnHitGE(const FPBItemModDef_GameplayEffect& Effect)
{
	GrantedEffectsToApplyOnHit.Add(Effect);
}

void FPBItemModDef_GrantedHandles::AddGrantedTexture(const FPBItemModDef_TextureOverride& Tex)
{
	GrantedTextureOverrides.Add(Tex);
}

void FPBItemModDef_GrantedHandles::AddActorSpawned(AActor* Actor)
{
	ExtraActorsSpawned.Add(Actor);
}

void FPBItemModDef_GrantedHandles::RemoveModFromEquipmentInstance(UPBModifiedWeaponInstance* Instance)
{
	if(!(Instance->GetPawn() && Instance->GetPawn()->HasAuthority()))
	{
		return;
	}

	ULyraAbilitySystemComponent* PBASC = Instance->GetASCFromOwningPawn();

	check(PBASC);

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			PBASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		UE_LOGFMT(LogPBGame, Warning, "Check: Remove effect");
		if (Handle.IsValid())
		{
			UE_LOGFMT(LogPBGame, Warning, "Remove effect");
			PBASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (const FGameplayTag& CueTag : GrantedPersistingCues)
	{
		PBASC->RemoveGameplayCue(CueTag);
	}

	for (const FPBItemModDef_GameplayEffect& Effect : GrantedEffectsToApplyOnHit)
	{
		Instance->RemoveEffectOnHit(Effect.GameplayEffect);
	}

	for (const FPBItemModDef_DamageType& DamageType : GrantedDamageTypes)
	{
		Instance->RemoveDamageGE(DamageType.DamageGE);
	}


	UE_LOGFMT(LogPBGame, Warning, "Need to implement texture override add and remove");
	/*for (const FPBItemModDef_TextureOverride& Tex : GrantedTextureOverrides)
	{
		
	}*/

	UE_LOGFMT(LogPBGame, Warning, "Need to implement extraActors add and remove");
	/*for(AActor* Actor : ExtraActorsSpawned)
	{
		
	}*/

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedPersistingCues.Reset();
	GrantedEffectsToApplyOnHit.Reset();
	GrantedDamageTypes.Reset();
	GrantedTextureOverrides.Reset();
	ExtraActorsSpawned.Reset();
}

UPBItemModDefinition::UPBItemModDefinition(const FObjectInitializer& ObjectInitializer)
{
}

void UPBItemModDefinition::ApplyToEquipmentInstance(UPBModifiedWeaponInstance* Instance,
	FPBItemModDef_GrantedHandles* OutGrantedHandles, int Magnitude) const
{
	if (!(Instance->GetPawn() && Instance->GetPawn()->HasAuthority()))
	{
		return;
	}

	ULyraAbilitySystemComponent* PBASC = Instance->GetASCFromOwningPawn();

	check(PBASC);

	UE_LOGFMT(LogPBGame, Warning, "Need to implement extraActors add and remove");
	/*for(AActor* Actor : ExtraActorsSpawned)
	{

	}*/

	for (int32 AbilityIndex = 0; AbilityIndex < GameplayAbilitiesToGrant.Num(); AbilityIndex++)
	{
		const FPBItemModDef_GameplayAbility& AbilityToGrant = GameplayAbilitiesToGrant[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTemp, Error, TEXT("Granted Mod Gameplay ability in index %d isnt valid"), AbilityIndex)
				continue;
		}

		ULyraGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<ULyraGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, Magnitude);
		AbilitySpec.SourceObject = Instance;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = PBASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	for (int32 EffectIndex = 0; EffectIndex < GameplayEffectsToGrant.Num(); EffectIndex++)
	{
		const FPBItemModDef_GameplayEffect& EffectToGrant = GameplayEffectsToGrant[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogTemp, Error, TEXT("Granted mod Gameplay effect in index %d isnt valid"), EffectIndex)
				continue;
		}

		UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = PBASC->ApplyGameplayEffectToSelf(GameplayEffect, Magnitude, PBASC->MakeEffectContext());

		UE_LOGFMT(LogPBGame, Warning, "GE handle is: {0}", GameplayEffectHandle.ToString());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	for (int32 CueIndex = 0; CueIndex < GameplayCuesToPlay.Num(); CueIndex++)
	{
		const FPBItemModDef_GameplayCue& CueToPlay = GameplayCuesToPlay[CueIndex];

		for (const FGameplayTag& Tag : CueToPlay.GameplayCue.GameplayCueTags)
		{
			UE_LOG(LogTemp, Error, TEXT("Adding mod cue but Need to give gameplay cues context"))
			PBASC->AddGameplayCue(Tag, PBASC->MakeEffectContext());
		}

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayCue(CueToPlay.GameplayCue);
		}
	}

	for (int32 AbilityIndex = 0; AbilityIndex < EffectsToApplyOnHit.Num(); AbilityIndex++)
	{
		const FPBItemModDef_GameplayEffect& EffectToApply = EffectsToApplyOnHit[AbilityIndex];

		if (!IsValid(EffectToApply.GameplayEffect))
		{
			UE_LOG(LogTemp, Error, TEXT("Granted Mod Gameplay effect on hit in index %d isnt valid"), AbilityIndex)
				continue;
		}

		Instance->AddEffectOnHit(EffectToApply.GameplayEffect);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGrantedOnHitGE(EffectToApply);
		}
	}

	for (int32 AbilityIndex = 0; AbilityIndex < DamageTypesToGrant.Num(); AbilityIndex++)
	{
		const FPBItemModDef_DamageType& DamageType = DamageTypesToGrant[AbilityIndex];

		if (!IsValid(DamageType.DamageGE))
		{
			UE_LOG(LogTemp, Error, TEXT("Granted Mod Gameplay effect on hit in index %d isnt valid"), AbilityIndex)
				continue;
		}

		Instance->AddDamageGE(DamageType.DamageGE);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGrantedDamageGE(DamageType);
		}
	}
}

FPrimaryAssetId UPBItemModDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UPBAssetManager::ItemModItemType, GetFName());
}

float UPBItemModDefinition::GetDisplayMagnitude(float InLevel)
{
	if(bUseCurveMagnitude)
	{
		if(DisplayMagnitudeInfo.DisplayMagnitude.IsValid())
		{
			return DisplayMagnitudeInfo.DisplayMagnitude.GetValueAtLevel(InLevel);
		}

		//if(GameplayEffectsToGrant.IsValidIndex(DisplayMagnitudeInfo.DisplayGEIndex))
		//{
		//	if (const UGameplayEffect* GE = GetDefault<UGameplayEffect>(GameplayEffectsToGrant[DisplayMagnitudeInfo.DisplayGEIndex].GameplayEffect))
		//	{
		//		if(const UPBGameplayEffectUIData_Magnitude* DataComp = GE->FindComponent<UPBGameplayEffectUIData_Magnitude>())
		//		{
		//			return DataComp->DisplayMagnitude.GetValueAtLevel(InLevel);
		//		}

		//		//if(GE->Modifiers.IsValidIndex(DisplayMagnitudeInfo.MagnitudeIndex))
		//		//{
		//		//	float outMag;
		//		//	if (GE->Modifiers[DisplayMagnitudeInfo.MagnitudeIndex].ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, outMag))
		//		//	{
		//		//		return outMag;
		//		//	}
		//		//}
		//	}
		//}
	}

	return DefaultDisplayMagnitude;
}

FText UPBItemModDefinition::GetModDescription(float inLevel)
{
	return FText::Format(Description, GetDisplayMagnitude(inLevel));
}
//
//EDataValidationResult UPBItemModDefinition::IsDataValid(FDataValidationContext& Context) const
//{
//	if(bUseCurveMagnitude && !DisplayMagnitudeInfo.DisplayMagnitude.IsValid())
//	{
//		return EDataValidationResult::Invalid;
//	}
//
//	return Super::IsDataValid(Context);
//}
