// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Launch/Resources/Version.h"
//#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
#include "Engine/HitResult.h"
//#else
//#include "Engine/EngineTypes.h"
//#endif
#include "LockOnSystemTargetComponent.generated.h"

class UIndicatorDescriptor;
class UUserWidget;
class UWidgetComponent;
class APlayerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LYRAGAME_API ULockOnSystemTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULockOnSystemTargetComponent();

	UFUNCTION(BlueprintCallable, Category = "LockOn|Target")
	void SetIsTargetable(bool isTarget);

	UFUNCTION(BlueprintCallable, Category = "LockOn|Target")
	bool GetIsTargetable();

	UFUNCTION(BlueprintCallable, Category = "LockOn|Target")
	FVector GetTargetLocation();

	UFUNCTION(BlueprintCallable, Category = "LockOn|Target")
	FVector GetTargetOffset();

	UFUNCTION(BlueprintCallable, Category = "LockOn|Target")
	void SetTargetOffset(FVector Offset);

	UFUNCTION(BlueprintCallable, Category = "LockOn|Target")
	FName GetSocketName();

private:

protected:
	UPROPERTY(EditDefaultsOnly, Category = "LockOn|Target")
	bool IsTargetable;

	// The Socket name to lock on to.
	//
	// Set it to None to lock on to the Root Component instead of the Mesh.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Target")
	FName LockedOnParentSocket;

	// The Relative Location to lock on when attached to a target.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockOn|Target")
	FVector LockedOnLocationOffset;
};
