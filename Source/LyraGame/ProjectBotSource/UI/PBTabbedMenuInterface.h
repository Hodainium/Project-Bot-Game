// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "PBTabbedMenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UPBTabbedMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LYRAGAME_API IPBTabbedMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tab Menu")
	void SetActiveTab(FName TabNameID, bool bSuppressClickFeedback = false);
};
