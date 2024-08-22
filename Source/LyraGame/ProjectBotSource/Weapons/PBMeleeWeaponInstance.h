// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PBModifiedWeaponInstance.h"
#include "Curves/CurveFloat.h"

#include "PBWeaponInstance.h"
#include "AbilitySystem/LyraAbilitySourceInterface.h"

#include "PBMeleeWeaponInstance.generated.h"

class UPhysicalMaterial;

/**
 * UPBMeleeWeaponInstance
 *
 * A piece of equipment representing a ranged weapon spawned and applied to a pawn. With modifiers enabled
 */
UCLASS()
class UPBMeleeWeaponInstance : public UPBModifiedWeaponInstance, public ILyraAbilitySourceInterface
{
	GENERATED_BODY()

public:
	UPBMeleeWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostLoad() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void UpdateDebugVisualization();
#endif

protected:
#if WITH_EDITORONLY_DATA
	//UPROPERTY(VisibleAnywhere, Category = "Spread|Fire Params")
	//float Debug_MinHeat = 0.0f;

	//UPROPERTY(VisibleAnywhere, Category = "Spread|Fire Params")
	//float Debug_MaxHeat = 0.0f;

	//UPROPERTY(VisibleAnywhere, Category="Spread|Fire Params", meta=(ForceUnits=deg))
	//float Debug_MinSpreadAngle = 0.0f;

	//UPROPERTY(VisibleAnywhere, Category="Spread|Fire Params", meta=(ForceUnits=deg))
	//float Debug_MaxSpreadAngle = 0.0f;

	//UPROPERTY(VisibleAnywhere, Category="Spread Debugging")
	//float Debug_CurrentHeat = 0.0f;

	//UPROPERTY(VisibleAnywhere, Category="Spread Debugging", meta = (ForceUnits=deg))
	//float Debug_CurrentSpreadAngle = 0.0f;

	//// The current *combined* spread angle multiplier
	//UPROPERTY(VisibleAnywhere, Category = "Spread Debugging", meta=(ForceUnits=x))
	//float Debug_CurrentSpreadAngleMultiplier = 1.0f;

#endif

	// List of special tags that affect how damage is dealt
	// These tags will be compared to tags in the physical material of the thing being hit
	// If more than one tag is present, the multipliers will be combined multiplicatively
	UPROPERTY(EditAnywhere, Category = "Weapon Config")
	TMap<FGameplayTag, float> MaterialDamageMultiplier;

public:
	void Tick(float DeltaSeconds);

	//~ULyraEquipmentInstance interface
	virtual void OnEquipped();
	virtual void OnUnequipped();
	//~End of ULyraEquipmentInstance interface

	//~ILyraAbilitySourceInterface interface
	virtual float GetDistanceAttenuation(float Distance, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const override;
	virtual float GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysicalMaterial, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const override;
	//~End of ILyraAbilitySourceInterface interface
};
