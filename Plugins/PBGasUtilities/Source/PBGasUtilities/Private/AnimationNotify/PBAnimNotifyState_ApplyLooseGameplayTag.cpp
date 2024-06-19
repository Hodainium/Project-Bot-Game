// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimationNotify/PBAnimNotifyState_ApplyLooseGameplayTag.h"

#include "AbilitySystemBlueprintLibrary.h"

UPBAnimNotifyState_ApplyLooseGameplayTag::UPBAnimNotifyState_ApplyLooseGameplayTag()
{
	bIsNativeBranchingPoint = true;
	RemoveTagsOnBlendOut = true;
	TagsAreApplied = false;
}

void UPBAnimNotifyState_ApplyLooseGameplayTag::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	AActor* Actor = MeshComp ? MeshComp->GetOwner() : nullptr;
	CachedOwningActor = Actor;
	CachedMontage = BranchingPointPayload.NotifyEvent->GetLinkedMontage();

	if (Actor)
	{
		TagsAreApplied = true;
		UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(Actor, GameplayTagsToApply, false);

		if(RemoveTagsOnBlendOut)
		{
			BranchingPointPayload.SkelMeshComponent->AnimScriptInstance->OnMontageBlendingOut.AddUniqueDynamic(this, &ThisClass::OnMontageBlendingOut);
		}
	}
}

void UPBAnimNotifyState_ApplyLooseGameplayTag::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	AActor* Actor = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (Actor)
	{
		if(TagsAreApplied)
		{
			TagsAreApplied = false;
			UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Actor, GameplayTagsToApply, false);
		}

		if (RemoveTagsOnBlendOut)
		{
			BranchingPointPayload.SkelMeshComponent->AnimScriptInstance->OnMontageBlendingOut.RemoveDynamic(this, &ThisClass::OnMontageBlendingOut);
		}
	}
}

#if WITH_EDITOR
bool UPBAnimNotifyState_ApplyLooseGameplayTag::CanBePlaced(UAnimSequenceBase* Animation) const
{
	return (Animation && Animation->IsA(UAnimMontage::StaticClass()));
}

void UPBAnimNotifyState_ApplyLooseGameplayTag::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage != CachedMontage.Get())
	{
		return;
	}

	if (TagsAreApplied)
	{
		AActor* Actor = CachedOwningActor.Get();
		if (Actor)
		{
			TagsAreApplied = false;
			UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Actor, GameplayTagsToApply, false);
		}
	}
}
#endif

FString UPBAnimNotifyState_ApplyLooseGameplayTag::GetNotifyName_Implementation() const
{
	return GameplayTagsToApply.ToStringSimple();
}
