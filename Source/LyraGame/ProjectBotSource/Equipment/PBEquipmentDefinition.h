// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PBEquipmentDefinition.generated.h"

class UPBEquipmentInstance;
class ULyraAbilitySet;

/**
 * 
 */
USTRUCT()
struct LYRAGAME_API FPBItemActorToSpawn
{
	GENERATED_BODY()

	FPBItemActorToSpawn()
	{}

	UPROPERTY(EditAnywhere, Category = "Equipment")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	FTransform AttachTransform;
};

//Need to change to equipmentdef
UCLASS(Const, BlueprintType)
class LYRAGAME_API UPBEquipmentDefinition : public UDataAsset
{
	GENERATED_BODY()

public:

	//What we actually spawn. Use for tracking world representation
	UPROPERTY(EditDefaultsOnly, Category="EquipmentDefinition|Instance")
	TSubclassOf<UPBEquipmentInstance> InstanceType;

	UPROPERTY(EditDefaultsOnly, Category="EquipmentDefinition|Abilities")
	TArray<TObjectPtr<const ULyraAbilitySet>> AbilitySetsToGrant;

	UPROPERTY(EditDefaultsOnly, Category = "EquipmentDefinition|ActorsToSpawn")
	TArray<FPBItemActorToSpawn> ActorsToSpawn;
};
