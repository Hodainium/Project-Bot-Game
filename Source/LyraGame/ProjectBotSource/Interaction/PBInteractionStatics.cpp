// Fill out your copyright notice in the Description page of Project Settings.

#include "PBInteractionStatics.h"

#include "Components/PrimitiveComponent.h"
#include "Engine/OverlapResult.h"
#include "IPBInteractableTarget.h"
#include "UObject/ScriptInterface.h"

UPBInteractionStatics::UPBInteractionStatics()
	: Super(FObjectInitializer::Get())
{
}

AActor* UPBInteractionStatics::GetActorFromInteractableTarget(TScriptInterface<IPBInteractableTarget> InteractableTarget)
{
	if (UObject* Object = InteractableTarget.GetObject())
	{
		if (AActor* Actor = Cast<AActor>(Object))
		{
			return Actor;
		}
		else if (UActorComponent* ActorComponent = Cast<UActorComponent>(Object))
		{
			return ActorComponent->GetOwner();
		}
		else
		{
			unimplemented();
		}
	}

	return nullptr;
}

void UPBInteractionStatics::GetInteractableTargetsFromActor(AActor* Actor, TArray<TScriptInterface<IPBInteractableTarget>>& OutInteractableTargets)
{
	// If the actor is directly interactable, return that.
	TScriptInterface<IPBInteractableTarget> InteractableActor(Actor);
	if (InteractableActor)
	{
		OutInteractableTargets.Add(InteractableActor);
	}

	// If the actor isn't interactable, it might have a component that has a interactable interface.
	TArray<UActorComponent*> InteractableComponents = Actor ? Actor->GetComponentsByInterface(UPBInteractableTarget::StaticClass()) : TArray<UActorComponent*>();
	for (UActorComponent* InteractableComponent : InteractableComponents)
	{
		OutInteractableTargets.Add(TScriptInterface<IPBInteractableTarget>(InteractableComponent));
	}
}

void UPBInteractionStatics::AppendInteractableTargetsFromOverlapResults(const TArray<FOverlapResult>& OverlapResults, TArray<TScriptInterface<IPBInteractableTarget>>& OutInteractableTargets)
{
	for (const FOverlapResult& Overlap : OverlapResults)
	{
		TScriptInterface<IPBInteractableTarget> InteractableActor(Overlap.GetActor());
		if (InteractableActor)
		{
			OutInteractableTargets.AddUnique(InteractableActor);
		}

		TScriptInterface<IPBInteractableTarget> InteractableComponent(Overlap.GetComponent());
		if (InteractableComponent)
		{
			OutInteractableTargets.AddUnique(InteractableComponent);
		}
	}
}

void UPBInteractionStatics::AppendInteractableTargetsFromHitResult(const FHitResult& HitResult, TArray<TScriptInterface<IPBInteractableTarget>>& OutInteractableTargets)
{
	TScriptInterface<IPBInteractableTarget> InteractableActor(HitResult.GetActor());
	if (InteractableActor)
	{
		OutInteractableTargets.AddUnique(InteractableActor);
	}

	TScriptInterface<IPBInteractableTarget> InteractableComponent(HitResult.GetComponent());
	if (InteractableComponent)
	{
		OutInteractableTargets.AddUnique(InteractableComponent);
	}
}