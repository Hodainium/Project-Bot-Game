// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AbilitySystem/Abilities/LyraGameplayAbility.h"

#include "PBGameplayAbility_DodgeRoll.generated.h"

struct FGameplayAbilityTargetData_PBComboInput;

class APawn;
class UObject;
struct FCollisionQueryParams;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;
struct FGameplayTag;
struct FGameplayTagContainer;



/**
 * UPBGameplayAbility_DodgeRoll
 *
 * An ability granted by and associated with a ranged weapon instance
 */
UCLASS()
class UPBGameplayAbility_DodgeRoll : public ULyraGameplayAbility
{
	GENERATED_BODY()

public:

	UPBGameplayAbility_DodgeRoll(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGameplayAbility interface

protected:

	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	// Called when target data is ready
	UFUNCTION(BlueprintImplementableEvent)
	void OnRollTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

	UFUNCTION(BlueprintCallable)
	void SetRollTargetData(FVector MovementInput, FVector ForwardVector);

private:
	FDelegateHandle OnTargetDataReadyCallbackDelegateHandle;
};
