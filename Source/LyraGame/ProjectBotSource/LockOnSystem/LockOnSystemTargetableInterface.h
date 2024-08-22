// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LockOnSystemTargetableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class ULockOnSystemTargetableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class LYRAGAME_API ILockOnSystemTargetableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Target System")
	bool IsTargetable() const;
};
