// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ProjectBotSource/LockOnSystem/LockOnSystemTargetableInterface.h"
#include "AbilitySystem/LyraAbilitySet.h"
#include "PBActorWithAbilities.generated.h"

class ULyraAbilitySet;
class ULyraAbilitySystemComponent;
class ULyraHealthComponent;

UCLASS(Blueprintable)
class LYRAGAME_API APBActorWithAbilities : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** AbilitySet this actor should be granted on spawn, if any */
	UPROPERTY(EditDefaultsOnly, Category = "PB|Ability")
	TObjectPtr<ULyraAbilitySet> AbilitySetOnSpawn;

public:
	// Set Class Defaults
	APBActorWithAbilities(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~AActor implementation
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor implementation

	/**
	 * This gets called when it's time to initialize the actor's ASC
	 *
	 * Override this as needed.  In your overrides, you MUST call Super()
	 * **BEFORE** you try to use the ASC at all.  The ASC is not functional
	 * before you call Super().
	 */
	virtual void InitializeAbilitySystem();

	/**
	 * This gets called when it's time to uninitialize the actor's ASC
	 */
	virtual void UninitializeAbilitySystem();

public:
	//~IAbilitySystemComponent interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	ULyraAbilitySystemComponent* GetLyraAbilitySystemComponentChecked() const;
	//~End IAbilitySystemComponent interface

private:
	// The ability system component for this actor
	UPROPERTY(EditDefaultsOnly, Category = "PB|Ability")
	TObjectPtr<ULyraAbilitySystemComponent> AbilitySystemComponent;

	// List of handles granted to this actor on spawn; these get cleaned up in EndPlay
	UPROPERTY(VisibleInstanceOnly, Category = "PB|Ability")
	FLyraAbilitySet_GrantedHandles GrantedHandlesOnSpawn;
};
