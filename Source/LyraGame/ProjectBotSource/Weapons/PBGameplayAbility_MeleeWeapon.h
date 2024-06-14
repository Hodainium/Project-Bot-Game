// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "ProjectBotSource/Equipment/PBGameplayAbility_FromEquipment.h"
#include "PBGameplayAbility_MeleeWeapon.generated.h"

struct FGameplayAbilityTargetData_PBMeleeInput;
class UPBMeleeWeaponInstance;
enum ECollisionChannel : int;

class APawn;
class UPBRangedWeaponInstance;
class UObject;
struct FCollisionQueryParams;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;
struct FGameplayTag;
struct FGameplayTagContainer;



/**
 * UPBGameplayAbility_MeleeWeapon
 *
 * An ability granted by and associated with a ranged weapon instance
 */
UCLASS()
class UPBGameplayAbility_MeleeWeapon : public UPBGameplayAbility_FromEquipment
{
	GENERATED_BODY()

public:

	UPBGameplayAbility_MeleeWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "PB|Ability")
	UPBMeleeWeaponInstance* GetMeleeWeaponInstance() const;

	//~UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGameplayAbility interface

protected:

	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	UFUNCTION(BlueprintCallable)
	void StartMeleeWeaponTracing();

	UFUNCTION(BlueprintCallable)
	void StopMeleeWeaponTracing();

	UFUNCTION()
	void OnWeaponTickFinished(const TArray<FHitResult>& InHits);

	// Called when target data is ready
	UFUNCTION(BlueprintImplementableEvent)
	void OnMeleeWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

	// Called when target data is ready
	UFUNCTION(BlueprintImplementableEvent)
	void OnComboTargetDataReady(uint8 Combo, float AttackAngle);

	UFUNCTION(BlueprintCallable)
	void SetComboAttackTargetData(uint8 Combo, float AttackAngle);

	UPROPERTY(BlueprintReadWrite)
	int CurrentCombo;

private:
	FDelegateHandle OnTargetDataReadyCallbackDelegateHandle;

	UPROPERTY()
	bool IsTracing;
};
