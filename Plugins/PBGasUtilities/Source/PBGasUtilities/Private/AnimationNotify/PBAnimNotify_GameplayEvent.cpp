// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimationNotify/PBAnimNotify_GameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Logging/StructuredLog.h"

UPBAnimNotify_GameplayEvent::UPBAnimNotify_GameplayEvent()
{
	bIsNativeBranchingPoint = true;
}

void UPBAnimNotify_GameplayEvent::BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Super::BranchingPointNotify(BranchingPointPayload);

	if (USkeletalMeshComponent* MeshComp = BranchingPointPayload.SkelMeshComponent)
	{
		if (AActor* Actor = MeshComp->GetOwner())
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, FGameplayEventData());
		}
	}
}

#if WITH_EDITOR
bool UPBAnimNotify_GameplayEvent::CanBePlaced(UAnimSequenceBase* Animation) const
{
	return (Animation && Animation->IsA(UAnimMontage::StaticClass()));
}
#endif

FString UPBAnimNotify_GameplayEvent::GetNotifyName_Implementation() const
{
	return EventTag.ToString();
}
