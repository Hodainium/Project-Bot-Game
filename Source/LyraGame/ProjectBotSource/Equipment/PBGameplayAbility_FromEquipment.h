// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Abilities/LyraGameplayAbility.h"

#include "PBGameplayAbility_FromEquipment.generated.h"

class UPBEquipmentInstance;
class UPBInventoryItemInstance;

/**
 * UPBGameplayAbility_FromEquipment
 *
 * An ability granted by and associated with an equipment instance
 */
UCLASS()
class UPBGameplayAbility_FromEquipment : public ULyraGameplayAbility
{
	GENERATED_BODY()

public:

	UPBGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="PB|Ability")
	UPBEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "PB|Ability")
	UPBInventoryItemInstance* GetAssociatedItem() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

};
