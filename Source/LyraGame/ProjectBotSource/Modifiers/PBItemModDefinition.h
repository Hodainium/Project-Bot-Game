// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "PBItemModDefinition.generated.h"

//Like fragments but 100% different. These define the base of mnodifiers we can recieve. Maybe should have their own fragments. 

enum class EPBItemQuality : uint8;
class ULyraAbilitySystemComponent;
class ULyraGameplayAbility;
class UGameplayEffect;
class UPBModifiedWeaponInstance;
struct FGameplayAbilitySpecHandle;
struct FActiveGameplayEffectHandle;
class UAttributeSet;
class UPBItemModInstance;
class UPBItemModFragment;
// DefaultToInstanced, EditInlineNew, Abstract

USTRUCT(BlueprintType)
struct FPBItemModDef_MagnitudeDisplay
{
	GENERATED_BODY()

public:
	//// Effect index in modDef
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//int32 DisplayGEIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat DisplayMagnitude;
};

USTRUCT(BlueprintType)
struct FPBItemModDef_GameplayAbility
{
	GENERATED_BODY()

public:

	// Gameplay ability to grant
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULyraGameplayAbility> Ability = nullptr;

	// Level of ability to grant
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;


	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};


USTRUCT(BlueprintType)
struct FPBItemModDef_GameplayEffect
{
	GENERATED_BODY()

public:

	// Gameplay effect to grant
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	// Level of gameplay effct
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

USTRUCT(BlueprintType)
struct FPBItemModDef_GameplayCue
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGameplayEffectCue GameplayCue;
};

USTRUCT(BlueprintType)
struct FPBItemModDef_TextureOverride
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* TextureOverrides;
};

USTRUCT(BlueprintType)
struct FPBItemModDef_DamageType
{
	GENERATED_BODY()

public:

	// Gameplay effect to grant
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageGE = nullptr;

	// Level of gameplay effct
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

USTRUCT(BlueprintType)
struct FPBItemModDef_ActorToSpawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Equipment")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	FTransform AttachTransform;
};

USTRUCT(BlueprintType)
struct FPBItemModDef_GrantedHandles
{
	GENERATED_BODY()

public:

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddGameplayCue(const FGameplayEffectCue& InCue);
	void AddGrantedDamageGE(const FPBItemModDef_DamageType& Effect);
	void AddGrantedOnHitGE(const FPBItemModDef_GameplayEffect& Effect);
	void AddGrantedTexture(const FPBItemModDef_TextureOverride& Tex);
	void AddActorSpawned(AActor* Actor);

	void RemoveModFromEquipmentInstance(UPBModifiedWeaponInstance* Instance);

protected:
	//Handles to granted abilities
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	//Handles to granted effects
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	//Granted gameplay cues
	UPROPERTY()
	FGameplayTagContainer GrantedPersistingCues;

	//Granted damageTypes
	UPROPERTY()
	TArray<FPBItemModDef_DamageType> GrantedDamageTypes;

	//Granted damageTypes
	UPROPERTY()
	TArray<FPBItemModDef_GameplayEffect> GrantedEffectsToApplyOnHit;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FPBItemModDef_TextureOverride> GrantedTextureOverrides;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<TObjectPtr<AActor>> ExtraActorsSpawned;
};

UCLASS(BlueprintType, Const)
class LYRAGAME_API UPBItemModDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPBItemModDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UFUNCTION(BlueprintCallable, Category = "Mods|Definition|Display")
	float GetDisplayMagnitude(float InLevel = 0.f);

	UFUNCTION(BlueprintCallable, Category="Mods|Definition|Display")
	FText GetModDescription(float inLevel = 0.f);

	//virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;

protected:
	//Need to remove just here for now. The function will just have mag=1
	void ApplyToEquipmentInstance(UPBModifiedWeaponInstance* Instance, FPBItemModDef_GrantedHandles* OutGrantedHandles, int Magnitude = 1) const;

	friend class UPBItemModInstance;

public:
	/** Text that describes the mod */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FText Title;

	/** Text that describes the mod */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	bool bDisplayInUI = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (ToolTip = "This is also the default value but also a fallback if magnitude fails"))
	float DefaultDisplayMagnitude = -1.;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (InlineEditConditionToggle))
	bool bUseCurveMagnitude = false;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (EditCondition = "bUseCurveMagnitude"))
	FPBItemModDef_MagnitudeDisplay DisplayMagnitudeInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable ,Category = "Quality", meta = (Categories = "Item.Quality"))
	FGameplayTagContainer AvailableQualities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable, Category = "Tags")
	FGameplayTagContainer AssetTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable, Category = "Tags")
	FGameplayTagContainer TagsToGrantDuringQuery;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable, Category = "Tags")
	FGameplayTagContainer RequiredTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable, Category = "Tags")
	FGameplayTagContainer BlockedTags;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FPBItemModDef_GameplayAbility> GameplayAbilitiesToGrant;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TArray<FPBItemModDef_GameplayEffect> GameplayEffectsToGrant;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Cues")
	TArray<FPBItemModDef_GameplayCue> GameplayCuesToPlay;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TArray<FPBItemModDef_DamageType> DamageTypesToGrant;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TArray<FPBItemModDef_GameplayEffect> EffectsToApplyOnHit;

	UPROPERTY(EditDefaultsOnly, Category = "Graphical")
	TArray<FPBItemModDef_TextureOverride> TextureOverrides;

	UPROPERTY(EditDefaultsOnly, Category = "Actors")
	TArray<FPBItemModDef_ActorToSpawn> ActorsToSpawn;
};



