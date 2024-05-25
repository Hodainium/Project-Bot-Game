// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimationNotify/PBAnimNotifyState_ApplyLooseGameplayTag.h"

#include "AbilitySystemBlueprintLibrary.h"

UPBAnimNotifyState_ApplyLooseGameplayTag::UPBAnimNotifyState_ApplyLooseGameplayTag()
{
	bIsNativeBranchingPoint = true;
}

void UPBAnimNotifyState_ApplyLooseGameplayTag::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	AActor* Actor = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (Actor)
	{
		UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(Actor, GameplayTagsToApply, false);
	}
}

void UPBAnimNotifyState_ApplyLooseGameplayTag::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);
	USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent;
	AActor* Actor = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (Actor)
	{
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Actor, GameplayTagsToApply, false);
	}
}

#if WITH_EDITOR
bool UPBAnimNotifyState_ApplyLooseGameplayTag::CanBePlaced(UAnimSequenceBase* Animation) const
{
	return (Animation && Animation->IsA(UAnimMontage::StaticClass()));
}
#endif

FString UPBAnimNotifyState_ApplyLooseGameplayTag::GetNotifyName_Implementation() const
{
	return GameplayTagsToApply.ToStringSimple();
}
