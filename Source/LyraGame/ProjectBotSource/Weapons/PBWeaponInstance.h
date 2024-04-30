// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Cosmetics/LyraCosmeticAnimationTypes.h"
#include "ProjectBotSource/Equipment/PBEquipmentInstance.h"
#include "PBWeaponInstance.generated.h"

class ULyraAbilitySystemComponent;
/**
 * Type of equipment applied to pawn
 */
UCLASS()
class LYRAGAME_API UPBWeaponInstance : public UPBEquipmentInstance
{
	GENERATED_BODY()

public:
	UPBWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//Equipment interface
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	////////////

	UFUNCTION(BlueprintCallable)
	void UpdateFiringTime();

	//Returns how long it has been since weapon was fired or equipped
	UFUNCTION(BlueprintPure)
	float GetTimeSinceLastInteractedWith() const;

	UFUNCTION(BlueprintCallable)
	ULyraAbilitySystemComponent* GetASCFromOwningPawn();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	FLyraAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	FLyraAnimLayerSelectionSet UnequippedAnimSet;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Animation")
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;

	

private:
	double TimeLastEquipped = 0.0;
	double TimeLastFired = 0.0;
};
