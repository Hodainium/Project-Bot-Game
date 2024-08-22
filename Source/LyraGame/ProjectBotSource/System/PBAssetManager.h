// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "System/LyraAssetManager.h"
#include "PBAssetManager.generated.h"

struct FPBItemSearchQuery;
/**
 * Game implementation of asset manager, overrides functionality and stores game-specific types
 * It is expected that most games will want to override AssetManager as it provides a good place for game-specific loading logic
 * This is used by setting AssetManagerClassName in DefaultEngine.ini
 */
UCLASS()
class LYRAGAME_API UPBAssetManager : public ULyraAssetManager
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UPBAssetManager();


	virtual void StartInitialLoading() override;

	/** Static types for items */
	static const FPrimaryAssetType	WeaponItemType;
	static const FPrimaryAssetType	ArmorItemType;
	static const FPrimaryAssetType	ResourceItemType;
	static const FPrimaryAssetType	ItemModItemType;
	static const FPrimaryAssetType	CharacterModItemType;
	static const FPrimaryAssetType	ConsumableItemType;
	static const FPrimaryAssetType	UseItemType;
	static const FPrimaryAssetType	UndefinedItemType;

	/** Returns the current AssetManager object */
	static UPBAssetManager& Get();

public:

	void GetAllItemModsMatching(FPBItemSearchQuery& SearchQuery, TArray<FAssetData>& OutAssets);
};

