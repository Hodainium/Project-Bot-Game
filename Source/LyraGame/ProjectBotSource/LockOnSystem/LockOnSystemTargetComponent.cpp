// Fill out your copyright notice in the Description page of Project Settings.

#include "LockOnSystemTargetComponent.h"


// Sets default values for this component's properties
ULockOnSystemTargetComponent::ULockOnSystemTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	LockedOnParentSocket = NAME_None;
	LockedOnLocationOffset = FVector(0.0f, 0.0f, 0.0f);
	IsTargetable = false;
}

void ULockOnSystemTargetComponent::SetIsTargetable(bool isTarget)
{
	IsTargetable = isTarget;
}

bool ULockOnSystemTargetComponent::GetIsTargetable()
{
	return IsTargetable;
}

FVector ULockOnSystemTargetComponent::GetTargetLocation()
{
	if(AActor* Owner = GetOwner())
	{
		if (UMeshComponent* Mesh = Owner->FindComponentByClass<UMeshComponent>())
		{
			return Mesh->GetSocketLocation(LockedOnParentSocket);
		}

		return Owner->GetActorLocation();
	}

	return FVector::Zero();
}

FVector ULockOnSystemTargetComponent::GetTargetOffset()
{
	return LockedOnLocationOffset;
}

void ULockOnSystemTargetComponent::SetTargetOffset(FVector Offset)
{
	LockedOnLocationOffset = Offset;
}

FName ULockOnSystemTargetComponent::GetSocketName()
{
	return LockedOnParentSocket;
}
