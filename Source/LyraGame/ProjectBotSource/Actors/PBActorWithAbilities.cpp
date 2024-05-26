// Fill out your copyright notice in the Description page of Project Settings.


#include "PBActorWithAbilities.h"

#include "ProjectBotSource/Actors/PBActorWithAbilitiesAndHealth.h"

#include "LyraLogChannels.h"
#include "AbilitySystem/LyraAbilitySet.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "Character/LyraHealthComponent.h"
#include "Logging/StructuredLog.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"


APBActorWithAbilities::APBActorWithAbilities(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Must replicate Actor for multiplayer
	bReplicates = true;

	AbilitySystemComponent = CreateDefaultSubobject<ULyraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
}


void APBActorWithAbilities::PostInitializeComponents()
{
	// BEFORE PostInit Components:
	InitializeAbilitySystem();

	// Now during PostInit Components, there is a functional ASC for other components to use
	Super::PostInitializeComponents();
}


void APBActorWithAbilities::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && AbilitySetOnSpawn)
	{
		UE_LOGFMT(LogPBGame, Warning, "Granting OnSpawn Ability Set {name}", GetNameSafe(AbilitySetOnSpawn)); //, GetNameSafe(AbilitySetOnSpawn

		AbilitySetOnSpawn->GiveToAbilitySystem(AbilitySystemComponent, &GrantedHandlesOnSpawn);
	}
}


void APBActorWithAbilities::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UninitializeAbilitySystem();

	Super::EndPlay(EndPlayReason);
}


void APBActorWithAbilities::InitializeAbilitySystem()
{
	UE_LOGFMT(LogPBGame, Warning, "Initializing Ability System");

	// We expect this to have been set in the constructor
	check(IsValid(AbilitySystemComponent));

	// Initialize ASC on this Actor
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}


void APBActorWithAbilities::UninitializeAbilitySystem()
{
	UE_LOGFMT(LogPBGame, Warning, "Uninitializing Ability System");

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAbilities();
		AbilitySystemComponent->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();
		AbilitySystemComponent->ClearActorInfo();

		GrantedHandlesOnSpawn.TakeFromAbilitySystem(AbilitySystemComponent);
	}
}


UAbilitySystemComponent* APBActorWithAbilities::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


ULyraAbilitySystemComponent* APBActorWithAbilities::GetLyraAbilitySystemComponentChecked() const
{
	check(AbilitySystemComponent);
	return AbilitySystemComponent;
}
