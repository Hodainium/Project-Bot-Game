// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "UObject/Object.h"
#include "PBAnimNotifyState_ApplyLooseGameplayTag.generated.h"

/**
 * 
 */
UCLASS()
class PBGASUTILITIES_API UPBAnimNotifyState_ApplyLooseGameplayTag : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPBAnimNotifyState_ApplyLooseGameplayTag();
	virtual void BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual void BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload) override;

	virtual FString GetNotifyName_Implementation() const override;

#if WITH_EDITOR
	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;
#endif

protected:
	UPROPERTY(EditAnywhere, Category=GameplayTags)
	FGameplayTagContainer GameplayTagsToApply;

	UPROPERTY(EditAnywhere, Category = GameplayTags)
	bool RemoveTagsOnBlendOut;

private:
	//This is used so that we can remove tags on onblendingout
	TWeakObjectPtr<AActor> CachedOwningActor;

	//This is used so that we can ensure that this montage is blending out in onblendingout
	TWeakObjectPtr<const UAnimMontage> CachedMontage;

	//A flag that indicates whether the tags are currently applied. Necessary because there are two ways the tags cna be removed.
	UPROPERTY(Transient)
	bool TagsAreApplied;

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
};
