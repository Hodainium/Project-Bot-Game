// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "NativeGameplayTags.h"
#include "PBStartingStatsAsset.h"
#include "AbilitySystem/LyraAbilitySet.h"
#include "PBStatComponent.generated.h"

struct FOnAttributeChangeData;
class UPBStatDefinition;
class UPBItemDefinition;
class UPBInventoryComponent;
struct FPBInventoryList;
class UPBInventoryItemInstance;

//UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_Item_Count, "ItemStatTags.Inventory.UseItem.Count");

USTRUCT(BlueprintType)
struct LYRAGAME_API FPBStatLevel
{
	GENERATED_BODY()

	FPBStatLevel() : Level(0)
	{
	}

public:

	int GetCost() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPBStatDefinition> StatDef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 Level;

	bool operator==(const FPBStatLevel& Other) const 
	{
		return (StatDef == Other.StatDef) && (Level == Other.Level);
	}
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FPBStatGrantedHandles
{
	GENERATED_BODY()

	FPBStatGrantedHandles(): StatLevel(0)
	{
	}

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 StatLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLyraAbilitySet_GrantedHandles GrantedAbilitySetHandles;
};

USTRUCT(BlueprintType)
struct LYRAGAME_API FPBStatInstance
{
	GENERATED_BODY()

	FPBStatInstance()
	{
	}

	FPBStatInstance(UPBStatDefinition* InStatDef)
	{
		StatDef = InStatDef;
	}

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPBStatDefinition> StatDef;

	FDelegateHandle StatLevelListener;

	FDelegateHandle MaxStatLevelListener;

	//Handles are for server only. No need to be replicated /////

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, NotReplicated)
	TArray<FPBStatGrantedHandles> GrantedAbilitySetHandles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, NotReplicated)
	TArray<FActiveGameplayEffectHandle> GrantedStatGEs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, NotReplicated)
	TArray<FActiveGameplayEffectHandle> GrantedMaxStatGEs;

	//////////////////////////////////////////////////////////////

	bool ContainsLevel(int LevelToCheck) const;

	int GetHandlesIndexForLevel(int LevelToGet);
	
};

UCLASS(BlueprintType)
class LYRAGAME_API UPBStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPBStatComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Stats|Button")
	void RequestStatPowerUp(UPBStatDefinition* StatDef);

	UFUNCTION(BlueprintCallable, Category = "Stats|Button")
	void RequestStatPowerDown(UPBStatDefinition* StatDef);

	UFUNCTION(BlueprintCallable, Category = "Stats|Button")
	bool IsPowerRequestValid(UPBStatDefinition* StatDef, int PowerChangeAmount);

	//TODO Need to add check against max here
	UFUNCTION(BlueprintCallable, Category = "Stats|Cart")
	bool TryAddToCart(FPBStatLevel CartEntry);

	UFUNCTION(BlueprintCallable, Category="Stats|Cart")
	void RemoveFromCart(FPBStatLevel CartEntry);

	UFUNCTION(BlueprintCallable, Category = "Stats|Cart")
	void RequestCartCheckout();

	UFUNCTION(BlueprintCallable, Category = "Stats|Cart")
	int GetCartCost();

	UFUNCTION(BlueprintCallable, Category = "Stats|Network")
	bool GetIsPendingServerConfirmation();

	UFUNCTION(BlueprintCallable, Category = "Stats|Stats")
	int GetStatLevelForStatDef(UPBStatDefinition* StatDef);

	UFUNCTION(BlueprintCallable, Category = "Stats|Stats")
	int GetMaxStatLevelForStatDef(UPBStatDefinition* StatDef);

	UFUNCTION(BlueprintCallable, Category = "Stats|Stats")
	int GetPowerBankLevel();

	UFUNCTION(BlueprintCallable, Category = "Stats|Stats")
	int GetPowerBankMaxLevel();

protected:

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void Server_TryPowerChange(UPBStatDefinition* StatDef, int PowerChangeAmount);

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = Inventory)
	void Client_ConfirmPowerChange(bool bSuccessful);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = Inventory)
	void Server_TryCartCheckout(const TArray<FPBStatLevel>& InCart);

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = Inventory)
	void Client_ConfirmCartCheckout(bool bSuccessful);

	bool CheckCartCost(int CostToAdd = 0);

	

	///////////////////////////////////////////////////////

	//Power GE's

	void GrantStatLevelPowerGE(UPBStatDefinition* StatDef);

	void GrantStatMaxLevelPowerGE(UPBStatDefinition* StatDef);

	void RemoveStatLevelPowerGE(UPBStatDefinition* StatDef);

	void RemoveStatMaxLevelPowerGE(UPBStatDefinition* StatDef);

	///////////////////////////////////////////////////////

	//Linking ASC TODO not hooked up anywhere yet

	void LinkASC(UAbilitySystemComponent* InASC);

	void UnLinkASC();

	virtual void HandleStatLevelAttributeChanged(const FOnAttributeChangeData& Data);

	virtual void HandleMaxStatLevelAttributeChanged(const FOnAttributeChangeData& Data);

	////////////////////////////////////////////////////////

	int FindStatInstanceIndexForAttribute(FGameplayAttribute Attribute);

	UPBInventoryComponent* GetInventoryComponentFromOwner();

	int GetStatInstanceIndex(UPBStatDefinition* StatDef);

	FPBStatGrantedHandles GrantStatAbilitySet(UPBStatDefinition* StatDef, int StatLevel);

	void RemoveStatAbilitySet(int InstanceIndex, int StatLevel);

	void GrantInitialStatsAsset();

	void GrantInitialStat(const FPBInitialStat& InInitialStat);

protected:

	UPROPERTY()
	bool IsPendingServerConfirmation;

	UPROPERTY()
	TArray<FPBStatLevel> Cart;

	UPROPERTY()
	UAbilitySystemComponent* LinkedASC;

	//This is what holds the listeners and handles for each active stat
	UPROPERTY()
	TArray<FPBStatInstance> StatInstances;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPBStartingStatsAsset> InitialStatsAsset;
};
