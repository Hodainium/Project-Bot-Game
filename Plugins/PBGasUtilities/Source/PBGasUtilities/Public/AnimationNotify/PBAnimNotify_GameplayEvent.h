// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "UObject/Object.h"
#include "PBAnimNotify_GameplayEvent.generated.h"

/**
 * Sends out a Gameplay Event Tag when this notify is reached.
 */
UCLASS(MinimalAPI)
class UPBAnimNotify_GameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPBAnimNotify_GameplayEvent();
	virtual void BranchingPointNotify(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual FString GetNotifyName_Implementation() const override;

#if WITH_EDITOR
	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const override;
#endif

protected:
	UPROPERTY(EditAnywhere, Category=GameplayEvent)
	FGameplayTag EventTag;
};
