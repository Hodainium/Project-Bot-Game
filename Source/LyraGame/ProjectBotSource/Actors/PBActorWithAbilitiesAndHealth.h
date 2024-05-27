// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "PBActorWithAbilities.h"
#include "GameFramework/Actor.h"
#include "ProjectBotSource/LockOnSystem/LockOnSystemTargetableInterface.h"
#include "PBActorWithAbilitiesAndHealth.generated.h"

class ULockOnSystemTargetComponent;
class ULyraCombatSet;
class ULyraHealthSet;
class ULyraAbilitySet;
class ULyraAbilitySystemComponent;
class ULyraHealthComponent;

UCLASS(Blueprintable)
class LYRAGAME_API APBActorWithAbilitiesAndHealth : public APBActorWithAbilities
{
	GENERATED_BODY()

public:
	// Set Class Defaults
	APBActorWithAbilitiesAndHealth(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~APBActorWithAbilities implementation
	virtual void InitializeAbilitySystem() override;
	virtual void UninitializeAbilitySystem() override;
	//~End of APBActorWithAbilities implementation

	// Begins the death sequence for the character (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	// Ends the death sequence for the character (detaches controller, destroys pawn, etc...)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDeathStarted"))
	void K2_OnDeathStarted();

	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDeathFinished"))
	void K2_OnDeathFinished();

protected:
	/**
	 * Lyra "Combat Set" is required by Lyra Healing & Damage Effect Calculations.
	 *
	 * This set defines base healing or damage values.
	 *
	 * It should probably be optional (it defaults to all zeros),
	 * but if it does not exist, Lyra will often complain via warning log messages.
	 *
	 * By default we just use all zeros for these attributes.
	 *
	 * Override as needed.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "PB|Ability")
	TObjectPtr<ULyraCombatSet> CombatSet;

	/**
	 * Lyra "Health Set" is required by Lyra Healing & Damage Effect Calculations.
	 *
	 * This defines health, max health, incoming healing and incoming damage.
	 *
	 * You probably want to derive your own class from this to add your custom
	 * attributes to this one.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "PB|Ability")
	TObjectPtr<ULyraHealthSet> HealthSet;

	/**
	 * Lyra "Health Component" is a convenience interface for HealthSet
	 *
	 * This requires the HealthSet and plugs into its event handlers.
	 * This makes it easier to respond to these events in BPs, for example.
	 *
	 * This component also seems to be what is responsible for handling the
	 * death of an Actor via network replication.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "PB|Ability")
	TObjectPtr<ULyraHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PB|LockOn", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULockOnSystemTargetComponent> TargetComponent;

};
