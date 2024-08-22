#include "GAT_PBGrantNearbyInteraction.h"

#include "AbilitySystemComponent.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"
#include "Physics/LyraCollisionChannels.h"
#include "ProjectBotSource/Interaction/IPBInteractableTarget.h"
#include "ProjectBotSource/Interaction/PBInteractionOption.h"
#include "ProjectBotSource/Interaction/PBInteractionQuery.h"
#include "ProjectBotSource/Interaction/PBInteractionStatics.h"

UGAT_PBGrantNearbyInteraction::UGAT_PBGrantNearbyInteraction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UGAT_PBGrantNearbyInteraction* UGAT_PBGrantNearbyInteraction::GrantAbilitiesForNearbyInteractors(UGameplayAbility* OwningAbility, float InteractionScanRange, float InteractionScanRate)
{
	UGAT_PBGrantNearbyInteraction* MyObj = NewAbilityTask<UGAT_PBGrantNearbyInteraction>(OwningAbility);
	MyObj->InteractionScanRange = InteractionScanRange;
	MyObj->InteractionScanRate = InteractionScanRate;
	return MyObj;
}

void UGAT_PBGrantNearbyInteraction::Activate()
{
	SetWaitingOnAvatar();

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(QueryTimerHandle, this, &ThisClass::QueryInteractables, InteractionScanRate, true);
}

void UGAT_PBGrantNearbyInteraction::OnDestroy(bool AbilityEnded)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(QueryTimerHandle);
	}

	Super::OnDestroy(AbilityEnded);
}

void UGAT_PBGrantNearbyInteraction::QueryInteractables()
{
	UWorld* World = GetWorld();
	AActor* ActorOwner = GetAvatarActor();

	if (World && ActorOwner)
	{
		FCollisionQueryParams Params(SCENE_QUERY_STAT(UGAT_PBGrantNearbyInteraction), false);

		TArray<FOverlapResult> OverlapResults;
		World->OverlapMultiByChannel(OUT OverlapResults, ActorOwner->GetActorLocation(), FQuat::Identity, Lyra_TraceChannel_Interaction, FCollisionShape::MakeSphere(InteractionScanRange), Params);

		if (OverlapResults.Num() > 0)
		{
			TArray<TScriptInterface<IPBInteractableTarget>> InteractableTargets;
			UPBInteractionStatics::AppendInteractableTargetsFromOverlapResults(OverlapResults, OUT InteractableTargets);

			FPBInteractionQuery InteractionQuery;
			InteractionQuery.RequestingAvatar = ActorOwner;
			InteractionQuery.RequestingController = Cast<AController>(ActorOwner->GetOwner());

			TArray<FPBInteractionOption> Options;
			for (TScriptInterface<IPBInteractableTarget>& InteractiveTarget : InteractableTargets)
			{
				FPBInteractionOptionBuilder InteractionBuilder(InteractiveTarget, Options);
				InteractiveTarget->GatherInteractionOptions(InteractionQuery, InteractionBuilder);
			}

			// Check if any of the options need to grant the ability to the user before they can be used.
			for (FPBInteractionOption& Option : Options)
			{
				if (Option.InteractionAbilityToGrant)
				{
					// Grant the ability to the GAS, otherwise it won't be able to do whatever the interaction is.
					FObjectKey ObjectKey(Option.InteractionAbilityToGrant);
					if (!InteractionAbilityCache.Find(ObjectKey))
					{
						FGameplayAbilitySpec Spec(Option.InteractionAbilityToGrant, 1, INDEX_NONE, this);
						FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
						InteractionAbilityCache.Add(ObjectKey, Handle);
					}
				}
			}
		}
	}
}

