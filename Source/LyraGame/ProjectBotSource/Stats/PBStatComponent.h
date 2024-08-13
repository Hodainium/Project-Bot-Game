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

USTRUCT(BlueprintType)
struct FPBStatDefChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<const UActorComponent> StatComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UPBStatDefinition> StatDef;
};

UCLASS(Blueprintable, BlueprintType, Meta = (BlueprintSpawnableComponent))
class LYRAGAME_API UPBStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPBStatComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Stats|Button")
	bool RequestStatPowerUp(UPBStatDefinition* StatDef);

	UFUNCTION(BlueprintCallable, Category = "Stats|Button")
	bool RequestStatPowerDown(UPBStatDefinition* StatDef);

	UFUNCTION(BlueprintCallable, Category = "Stats|Button")
	bool IsPowerRequestValid(UPBStatDefinition* StatDef, int PowerChangeAmount);

	UFUNCTION(BlueprintCallable, Category = "Stats|Button")
	bool IsMaxPowerRequestValid(UPBStatDefinition* StatDef, int PowerChangeAmount = 1);

	UFUNCTION(BlueprintCallable, Category = "Stats|Button")
	bool IsCartEntryValid(const FPBStatLevel& CartEntry);

	UFUNCTION(BlueprintCallable, Category = "Stats|Cart")
	bool TryAddToCart(FPBStatLevel CartEntry);

	UFUNCTION(BlueprintCallable, Category="Stats|Cart")
	void RemoveFromCart(FPBStatLevel CartEntry);

	UFUNCTION(BlueprintCallable, Category = "Stats|Cart")
	void RequestCartCheckout();

	UFUNCTION(BlueprintCallable, Category = "Stats|Cart")
	void EmptyCart();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats|Cart")
	int GetCartCost();

	UFUNCTION(BlueprintCallable, BlueprintPure , Category = "Stats|Network")
	bool GetIsPendingServerConfirmation();

	UFUNCTION(BlueprintCallable, Category = "Stats|Cart")
	bool GetHighestCartEntryForStatDef(UPBStatDefinition* StatDef, FPBStatLevel& CartEntry);

	UFUNCTION(BlueprintCallable, Category = "Stats|Cart")
	FPBStatLevel CreateNextHighestCartEntryForStatDef(UPBStatDefinition* StatDef);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats|Stats")
	int GetStatLevelForStatDef(UPBStatDefinition* StatDef);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats|Stats")
	int GetMaxStatLevelForStatDef(UPBStatDefinition* StatDef);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats|Stats")
	int GetPowerBankLevel();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats|Stats")
	int GetPowerBankMaxLevel();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats|Button")
	int GetCartCountForStatDefinition(UPBStatDefinition* StatDef);

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "Stats|ASC")
	void InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC);

	// Uninitialize the component, clearing any references to the ability system. And removing abilities
	UFUNCTION(BlueprintCallable, Category = "Stats|ASC")
	void UninitializeFromAbilitySystem();

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


	//TODO This is where we broadcast
	virtual void HandleStatLevelAttributeChanged(const FOnAttributeChangeData& Data);

	virtual void HandleMaxStatLevelAttributeChanged(const FOnAttributeChangeData& Data);

	////////////////////////////////////////////////////////

	int FindStatInstanceIndexForAttribute(FGameplayAttribute Attribute);

	UPBInventoryComponent* GetInventoryComponentFromOwner();

	int GetStatInstanceIndex(UPBStatDefinition* StatDef);

	FPBStatGrantedHandles GrantStatAbilitySet(UPBStatDefinition* StatDef, int StatLevel);

	void RemoveStatAbilitySet(int InstanceIndex, int StatLevel);

	void GrantInitialStatsAsset();

	void RemoveAllStatInstances();

	void GrantInitialStat(const FPBInitialStat& InInitialStat);

	void RemoveStatInstance(FPBStatInstance& InStatInstance);

	UPBStatDefinition* GetStatDefinitionForAttribute(const FGameplayAttribute& InAttribute) const;

	void BroadcastStatAttributeChangeMessage(const FGameplayAttribute& InAttribute) const;

	void BroadcastStatAttributeChangeMessage(UPBStatDefinition* InStatDef) const;

protected:

	UPROPERTY()
	bool IsPendingServerConfirmation;

	UPROPERTY()
	TArray<FPBStatLevel> Cart;

	UPROPERTY()
	ULyraAbilitySystemComponent* LinkedASC;

	//This is what holds the listeners and handles for each active stat
	UPROPERTY()
	TArray<FPBStatInstance> StatInstances;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPBStartingStatsAsset> InitialStatsAsset;
};
