// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Components/ActorComponent.h"
#include "PBWeaponComponent.generated.h"


class UPBInventoryItemInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPBMeleeAttackDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPBPlayHitMarker);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPBOnAttackHitEvent, TArray<FHitResult>&, Hit, UPrimitiveComponent*, WeaponMesh);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FPBOnMeleEPBitSignature,
	class AActor*, HitActor, class UPrimitiveComponent*, HitComponent, const FVector&, ImpactPoint, const FVector&, ImpactNormal,
	const FGameplayTag&, WeaponPartTag, FName, HitBoneName, const struct FHitResult&, HitResult);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LYRAGAME_API UPBWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPBWeaponComponent();

	void RegisterWithAbilitySystem(UAbilitySystemComponent* ASC);

	void UnregisterWithAbilitySystem();

protected:

	void ActuallyUpdateDamageInstigatedTime();

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UPBInventoryItemInstance>> EquippedItems;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UPBInventoryItemInstance> CurrentEquippedItem;

	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponentRef;

public:	

	UFUNCTION(BlueprintCallable, Category = "Weapon|Abilities")
	bool CanAttack() const;

	UFUNCTION(Client, Reliable)
	void ClientConfirmTargetData(uint16 UniqueId, bool bSuccess, const TArray<uint8>& HitReplaces);

	void AddUnconfirmedServerSidEPBitMarkers(const FGameplayAbilityTargetDataHandle& InTargetData, const TArray<FHitResult>& FoundHits);

	UFUNCTION(BlueprintCallable, Category = "Weapon|Melee")
	void HandleOnMeleEPBitEvent(AActor* HitActor, UPrimitiveComponent* HitComponent, const FVector& ImpactPoint, const FVector& ImpactNormal, const FGameplayTag& WeaponPartTag, FName HitBoneName, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category = "Weapon|Melee|Trace")
	void StartMeleeTrace();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Melee|Trace")
	void StopMeleeTrace();

	UPROPERTY(BlueprintReadWrite)
	float TimeOfLastAttack;

	UPROPERTY(BlueprintReadWrite)
	float AttackCooldownTime;

	UPROPERTY(BlueprintAssignable, Category = "Melee Weapon|Event")
	FPBOnMeleEPBitSignature OnMeleeHitTaggedEvent;

public:

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Melee|Events")
	FPBMeleeAttackDelegate OnMeleeTraceStart;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Melee|Events")
	FPBMeleeAttackDelegate OnMeleeTraceStop;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Melee|Events")
	FPBOnAttackHitEvent OnMeleeHitFound;

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
	FPBPlayHitMarker PlayHitMarkerDelegate;

protected:

	UPROPERTY()
	TArray<uint8> UnconfirmedMeleeHitMarkers;

private:
	
};
