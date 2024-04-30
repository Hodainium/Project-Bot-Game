// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBItemModInstance.generated.h"

enum class EPBItemQuality : uint8;
struct FPBItemModDef_GrantedHandles;
class UPBModifiedWeaponInstance;
class UPBItemModDefinition;

UCLASS(BlueprintType)
class LYRAGAME_API UPBItemModInstance : public UObject
{
	GENERATED_BODY()

public:
	UPBItemModInstance();

	UFUNCTION(BlueprintCallable)
	UPBItemModDefinition* GetModDefinition() const;

	UFUNCTION(BlueprintCallable)
	EPBItemQuality GetModQuality() const;

	UFUNCTION(BlueprintCallable, Category="Mods|Display")
	float GetDisplayMagnitude() const;

	UFUNCTION(BlueprintCallable, Category = "Mods|Display")
	FText GetDisplayDescription() const;

	UFUNCTION(BlueprintCallable)
	float GetModLevel() const;

	//TODO eventually make this accept Uobjects and check if they implement a certain interface?
	void OnWeaponEquipped(UPBModifiedWeaponInstance* EquipmentInstance, FPBItemModDef_GrantedHandles* OutGrantedHandles);
	
protected:

	friend class UPBLootGenGameInstanceSubsystem;

	void SetModLevelOffset(float inLevelOffset);

	void SetModDefinition(UPBItemModDefinition* InDef);

	void SetModQuality(EPBItemQuality InQuality);

	/** ItemMod definition. The static data that this instance points to. */
	UPROPERTY()
	TObjectPtr<UPBItemModDefinition> ModDef;

	UPROPERTY()
	float LevelOffset;

	UPROPERTY()
	EPBItemQuality ModQuality;

	//void RemoveFromWeaponInstance(UPBModifiedWeaponInstance* EquipmentInstance, FPBItemModDef_GrantedHandles* GrantedHandles);
};