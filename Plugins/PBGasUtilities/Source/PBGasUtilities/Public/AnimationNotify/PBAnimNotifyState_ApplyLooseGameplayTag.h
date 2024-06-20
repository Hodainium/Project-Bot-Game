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


private:
};
