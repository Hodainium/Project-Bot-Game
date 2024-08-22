// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimationNotify/PBAnimNotifyState_StartEndGameplayEvents.h"

#include "AbilitySystemBlueprintLibrary.h"

UPBAnimNotifyState_StartEndGameplayEvents::UPBAnimNotifyState_StartEndGameplayEvents()
{
	bIsNativeBranchingPoint = true;
}

void UPBAnimNotifyState_StartEndGameplayEvents::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyBegin(BranchingPointPayload);

	if (USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent)
	{
		if (AActor* Actor = MeshComp->GetOwner())
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, StartEventTag, FGameplayEventData());
		}
	}
}

void UPBAnimNotifyState_StartEndGameplayEvents::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotifyEnd(BranchingPointPayload);

	if (USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent)
	{
		if (AActor* Actor = MeshComp->GetOwner())
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EndEventTag, FGameplayEventData());
		}
	}
}

#if WITH_EDITOR
bool UPBAnimNotifyState_StartEndGameplayEvents::CanBePlaced(UAnimSequenceBase* Animation) const
{
	return (Animation && Animation->IsA(UAnimMontage::StaticClass()));
}
#endif

FString UPBAnimNotifyState_StartEndGameplayEvents::GetNotifyName_Implementation() const
{
	return StartEventTag.ToString() + ", " + EndEventTag.ToString();
}
