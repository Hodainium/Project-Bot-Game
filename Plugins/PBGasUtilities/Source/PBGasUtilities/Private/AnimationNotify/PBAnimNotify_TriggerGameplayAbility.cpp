// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimationNotify/PBAnimNotify_TriggerGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
//#include "AbilitySystem/PBUtilitiesBlueprintLibrary.h"

UPBAnimNotify_TriggerGameplayAbility::UPBAnimNotify_TriggerGameplayAbility(): bSetNextMontageSectionIfAbilityFailed(false)
{
	bIsNativeBranchingPoint = true;
}

void UPBAnimNotify_TriggerGameplayAbility::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	AActor* Actor = MeshComp ? MeshComp->GetOwner() : nullptr;
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
	if (!ASC)
	{
		HandleJumpSection(MeshComp, nullptr, Cast<UAnimMontage>(BranchingPointPayload.SequenceAsset));
		return;
	}

	if (ASC->IsOwnerActorAuthoritative())
	{
		TArray<FGameplayAbilitySpec*> OutSpecs;
		ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(AbilityTriggerTag), OutSpecs);
		for (const FGameplayAbilitySpec* Spec : OutSpecs)
		{
			const bool bValidAbility = Spec->Ability != nullptr;
			const bool bTagsPass = Spec->Ability->AbilityTags.HasAll(FGameplayTagContainer(AbilityTriggerTag));
			if (bValidAbility && bTagsPass && ASC->TryActivateAbility(Spec->Handle, false))
			{
				//We are done, 
				return;
			}
		}

		HandleJumpSection(MeshComp, ASC, Cast<UAnimMontage>(BranchingPointPayload.SequenceAsset));
	}
}

FString UPBAnimNotify_TriggerGameplayAbility::GetNotifyName_Implementation() const
{
	return AbilityTriggerTag.ToString();
}

#if WITH_EDITOR
bool UPBAnimNotify_TriggerGameplayAbility::CanBePlaced(UAnimSequenceBase* Animation) const
{
	return (Animation && Animation->IsA(UAnimMontage::StaticClass()));
}
#endif

void UPBAnimNotify_TriggerGameplayAbility::HandleJumpSection(USkeletalMeshComponent* MeshComponent, UAbilitySystemComponent* ASC, UAnimMontage* CurrentMontage)
{
	//Should we handle GAS(?)
	if (bSetNextMontageSectionIfAbilityFailed)
	{
		const FName CurrentMontageSectionName = MeshComponent->GetAnimInstance()->Montage_GetCurrentSection(CurrentMontage);
		if (ASC && ASC->GetCurrentMontage() == CurrentMontage)
		{
			ASC->CurrentMontageSetNextSectionName(CurrentMontageSectionName, NextMontageSectionIfAbilityFailed);
		}
		else
		{
			MeshComponent->GetAnimInstance()->Montage_SetNextSection(CurrentMontageSectionName, NextMontageSectionIfAbilityFailed, CurrentMontage);
		}
	}
}
