#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/OverlapResult.h"
#include "PBInteractionStatics.generated.h"

template <typename InterfaceType> class TScriptInterface;

class AActor;
class IPBInteractableTarget;
class UObject;
struct FFrame;
struct FHitResult;
//struct FOverlapResult;

/**  */
UCLASS()
class UPBInteractionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UPBInteractionStatics();

public:
	UFUNCTION(BlueprintCallable)
	static AActor* GetActorFromInteractableTarget(TScriptInterface<IPBInteractableTarget> InteractableTarget);

	UFUNCTION(BlueprintCallable)
	static void GetInteractableTargetsFromActor(AActor* Actor, TArray<TScriptInterface<IPBInteractableTarget>>& OutInteractableTargets);

	static void AppendInteractableTargetsFromOverlapResults(const TArray<FOverlapResult>& OverlapResults, TArray<TScriptInterface<IPBInteractableTarget>>& OutInteractableTargets);
	static void AppendInteractableTargetsFromHitResult(const FHitResult& HitResult, TArray<TScriptInterface<IPBInteractableTarget>>& OutInteractableTargets);
};
