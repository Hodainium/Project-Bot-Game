#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"
#include "UObject/Interface.h"

#include "UObject/ObjectPtr.h"
#include "IPBPickupable.generated.h"

template <typename InterfaceType> class TScriptInterface;

class AActor;
class UPBItemDefinition;
class UPBInventoryItemInstance;
class UPBInventoryComponent;
class UObject;
struct FFrame;

USTRUCT(BlueprintType)
struct FPBPickupTemplate
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StackCount = 1;

	//TODO: MAKE this into a primary asset id
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UPBItemDefinition> ItemDef;
};

USTRUCT(BlueprintType)
struct FPBPickupInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UPBInventoryItemInstance> Item = nullptr;
};

USTRUCT(BlueprintType)
struct FPBInventoryPickup
{
	GENERATED_BODY()

	FPBInventoryPickup()
	{
	}

	FPBInventoryPickup(UPBInventoryItemInstance* InInstance)
	{
		if(InInstance)
		{
			FPBPickupInstance Pickup = FPBPickupInstance(InInstance);
			Instances.Add(Pickup);
		}
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPBPickupInstance> Instances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPBPickupTemplate> Templates;
};

/**  */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UPBPickupable : public UInterface
{
	GENERATED_BODY()
};

/**  */
class IPBPickupable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual FPBInventoryPickup GetPickupInventory() const = 0;
};

/**  */
UCLASS()
class UPBPickupableStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UPBPickupableStatics();

public:
	UFUNCTION(BlueprintPure)
	static TScriptInterface<IPBPickupable> GetFirstPickupableFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "Ability"))
	static void AddPickupToInventory(UPBInventoryComponent* InventoryComponent, TScriptInterface<IPBPickupable> Pickup);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "Ability"))
	static void PushPickupToPlayer(APawn* PlayerPawn, TScriptInterface<IPBPickupable> Pickup);

private:
	//TODO finish async loading
	void OnWeaponLoad();
};
