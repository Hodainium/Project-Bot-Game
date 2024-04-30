// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Equipment/PBEquipmentInstance.h"
#include "ProjectBotSource/Equipment/PBEquipmentDefinition.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

UPBEquipmentInstance::UPBEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UWorld* UPBEquipmentInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}

	return nullptr;
}

void UPBEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}

void UPBEquipmentInstance::OnRep_Instigator()
{
}

APawn* UPBEquipmentInstance::GetPawn() const
{
	UObject* Outer = GetOuter();

	if(APlayerState* PS = Cast<APlayerState>(Outer))
	{
		return PS->GetPawn();
	}
	else
	{
		return Cast<APawn>(GetOuter());
	}
}

APawn* UPBEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if(UClass* ActualPawnType = PawnType)
	{
		if(APawn* Pawn = GetPawn())
		{
			if (Pawn->IsA(ActualPawnType))
			{
				Result = Pawn;
			}
		}
	}
	return Result;
}

void UPBEquipmentInstance::SpawnEquipmentActors(const TArray<FPBItemActorToSpawn>& ActorsToSpawn)
{
	if(APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = Char->GetMesh();
		}

		for (const FPBItemActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, true);
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);
			

			SpawnedActors.Add(NewActor);
		}
	}
}

void UPBEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if(Actor)
		{
			Actor->Destroy();
		}
	}
}

void UPBEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UPBEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}
