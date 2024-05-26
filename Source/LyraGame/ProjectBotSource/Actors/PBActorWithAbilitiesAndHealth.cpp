// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Actors/PBActorWithAbilitiesAndHealth.h"

#include "LyraLogChannels.h"
#include "AbilitySystem/LyraAbilitySet.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "AbilitySystem/Attributes/LyraCombatSet.h"
#include "Character/LyraHealthComponent.h"
#include "Logging/StructuredLog.h"


APBActorWithAbilitiesAndHealth::APBActorWithAbilitiesAndHealth(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CombatSet = CreateDefaultSubobject<ULyraCombatSet>("CombatSet");
	HealthSet = CreateDefaultSubobject<ULyraHealthSet>("HealthSet");

	HealthComponent = CreateDefaultSubobject<ULyraHealthComponent>("HealthComponent");
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	HealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);

	IsTargetable = false;
}

bool APBActorWithAbilitiesAndHealth::IsTargetable_Implementation() const
{
	return IsTargetable;
}


void APBActorWithAbilitiesAndHealth::InitializeAbilitySystem()
{
	// Super::InitializeAbilitySystem is required to get the ASC working, do this first:
	Super::InitializeAbilitySystem();

	// HealthComponent requires a Lyra based ASC
	ULyraAbilitySystemComponent* ASC = GetLyraAbilitySystemComponentChecked();

	// Add Attribute Sets to ASC
	ASC->AddAttributeSetSubobject(CombatSet.Get());
	ASC->AddAttributeSetSubobject(HealthSet.Get());

	// DO NOT init HealthComponent until AFTER HealthSet has been added
	HealthComponent->InitializeWithAbilitySystem(ASC);
}


void APBActorWithAbilitiesAndHealth::UninitializeAbilitySystem()
{
	HealthComponent->UninitializeFromAbilitySystem();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->RemoveSpawnedAttribute(CombatSet.Get());
		ASC->RemoveSpawnedAttribute(HealthSet.Get());
	}

	Super::UninitializeAbilitySystem();
}

void APBActorWithAbilitiesAndHealth::OnDeathStarted(AActor* OwningActor)
{
	IsTargetable = false;
	K2_OnDeathStarted();
}

void APBActorWithAbilitiesAndHealth::OnDeathFinished(AActor* OwningActor)
{
	K2_OnDeathFinished();
}
