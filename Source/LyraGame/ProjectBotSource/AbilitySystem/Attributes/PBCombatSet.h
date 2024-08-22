// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "PBCombatSet.generated.h"
/**
 * 
 */
UCLASS(BlueprintType)
class LYRAGAME_API UPBCombatSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:

	UPBCombatSet();

	ATTRIBUTE_ACCESSORS(UPBCombatSet, BaseKnockback);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UFUNCTION()
	void OnRep_BaseKnockback(const FGameplayAttributeData& OldValue);

private:

	// The base amount of BaseKnockback to apply in the BaseKnockback execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseKnockback, Category = "PB|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseKnockback;
};
