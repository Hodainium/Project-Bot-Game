// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Actors/PBActorWithAbilitiesAndHealth.h"

#include "LyraLogChannels.h"
#include "AbilitySystem/LyraAbilitySet.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "AbilitySystem/Attributes/LyraCombatSet.h"
#include "Character/LyraHealthComponent.h"
#include "Logging/StructuredLog.h"
#include "ProjectBotSource/AbilitySystem/Attributes/PBHealthSet.h"
#include "ProjectBotSource/LockOnSystem/LockOnSystemTargetComponent.h"


APBActorWithAbilitiesAndHealth::APBActorWithAbilitiesAndHealth(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CombatSet = CreateDefaultSubobject<ULyraCombatSet>("CombatSet");
	HealthSet = CreateDefaultSubobject<UPBHealthSet>("HealthSet");

	TargetComponent = CreateDefaultSubobject<ULockOnSystemTargetComponent>("TargetComponent");

	HealthComponent = CreateDefaultSubobject<ULyraHealthComponent>("HealthComponent");
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

	HealthComponent->OnDeathStarted.AddUniqueDynamic(this, &ThisClass::OnDeathStarted);
	HealthComponent->OnDeathFinished.AddUniqueDynamic(this, &ThisClass::OnDeathFinished);
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
	TargetComponent->SetIsTargetable(false);
	K2_OnDeathStarted();
}

void APBActorWithAbilitiesAndHealth::OnDeathFinished(AActor* OwningActor)
{
	K2_OnDeathFinished();
}
