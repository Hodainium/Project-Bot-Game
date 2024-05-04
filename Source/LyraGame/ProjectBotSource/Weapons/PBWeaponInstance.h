// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cosmetics/LyraCosmeticAnimationTypes.h"
#include "UObject/NoExportTypes.h"
#include "ProjectBotSource/Equipment/PBEquipmentInstance.h"
#include "PBWeaponInstance.generated.h"

class ULyraAbilitySystemComponent;
class UPBAbilitySystemComponent;
/**
 * Type of equipment applied to pawn
 */
UCLASS()
class LYRAGAME_API UPBWeaponInstance : public UPBEquipmentInstance
{
	GENERATED_BODY()

public:
	UPBWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UPBEquipmentInstance interface
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	//~End of UPBEquipmentInstance interface

	UFUNCTION(BlueprintCallable)
	void UpdateFiringTime();

	// Returns how long it's been since the weapon was interacted with (fired or equipped)
	UFUNCTION(BlueprintPure)
	float GetTimeSinceLastInteractedWith() const;

	UFUNCTION(BlueprintCallable)
	ULyraAbilitySystemComponent* GetASCFromOwningPawn();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLyraAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLyraAnimLayerSelectionSet UnequippedAnimSet;

	/**
	 * Device properties that should be applied while this weapon is equipped.
	 * These properties will be played in with the "Looping" flag enabled, so they will
	 * play continuously until this weapon is unequipped!
	 */
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = "Input Devices")
	TArray<TObjectPtr<UInputDeviceProperty>> ApplicableDeviceProperties;

	// Choose the best layer from EquippedAnimSet or UneuippedAnimSet based on the specified gameplay tags
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;

	/** Returns the owning Pawn's Platform User ID */
	UFUNCTION(BlueprintCallable)
	const FPlatformUserId GetOwningUserId() const;

	/** Callback for when the owning pawn of this weapon dies. Removes all spawned device properties. */
	UFUNCTION()
	void OnDeathStarted(AActor* OwningActor);

	/**
	 * Apply the ApplicableDeviceProperties to the owning pawn of this weapon.
	 * Populate the DevicePropertyHandles so that they can be removed later. This will
	 * Play the device properties in Looping mode so that they will share the lifetime of the
	 * weapon being Equipped.
	 */
	void ApplyDeviceProperties();

	/** Remove any device proeprties that were activated in ApplyDeviceProperties. */
	void RemoveDeviceProperties();

private:

	/** Set of device properties activated by this weapon. Populated by ApplyDeviceProperties */
	UPROPERTY(Transient)
	TSet<FInputDevicePropertyHandle> DevicePropertyHandles;

	double TimeLastEquipped = 0.0;
	double TimeLastFired = 0.0;

};


