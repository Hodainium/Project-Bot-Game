// Fill out your copyright notice in the Description page of Project Settings.


#include "PBWorldInteractable.h"

// Sets default values
APBWorldInteractable::APBWorldInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APBWorldInteractable::GatherInteractionOptions(const FPBInteractionQuery& InteractQuery,
	FPBInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}



