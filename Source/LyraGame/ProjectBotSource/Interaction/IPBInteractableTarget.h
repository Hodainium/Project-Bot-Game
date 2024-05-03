// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PBInteractionOption.h"
#include "IPBInteractableTarget.generated.h"

struct FPBInteractionQuery;

/**  */
class FPBInteractionOptionBuilder
{
public:
	FPBInteractionOptionBuilder(TScriptInterface<IPBInteractableTarget> InterfaceTargetScope, TArray<FPBInteractionOption>& InteractOptions)
		: Scope(InterfaceTargetScope)
		, Options(InteractOptions)
	{
	}

	void AddInteractionOption(const FPBInteractionOption& Option)
	{
		FPBInteractionOption& OptionEntry = Options.Add_GetRef(Option);
		OptionEntry.InteractableTarget = Scope;
	}

private:
	TScriptInterface<IPBInteractableTarget> Scope;
	TArray<FPBInteractionOption>& Options;
};

/**  */
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UPBInteractableTarget : public UInterface
{
	GENERATED_BODY()
};

/**  */
class IPBInteractableTarget
{
	GENERATED_BODY()

public:
	/**  */
	virtual void GatherInteractionOptions(const FPBInteractionQuery& InteractQuery, FPBInteractionOptionBuilder& OptionBuilder) = 0;

	/**  */
	virtual void CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag, FGameplayEventData& InOutEventData) { }
};
