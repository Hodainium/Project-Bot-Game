// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IPBInteractableTarget.h"
#include "PBInteractionOption.h"
#include "PBWorldInteractable.generated.h"

UCLASS()
class LYRAGAME_API APBWorldInteractable : public AActor, public IPBInteractableTarget
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APBWorldInteractable();

	virtual void GatherInteractionOptions(const FPBInteractionQuery& InteractQuery, FPBInteractionOptionBuilder& InteractionBuilder) override;

protected:

	UPROPERTY(EditAnywhere)
	FPBInteractionOption Option;

};
