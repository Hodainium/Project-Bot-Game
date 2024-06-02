// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Weapons/PBMeleeWeaponTraceComponent.h"

#include "MeleeSystem/PBMeleeCore.h"
#include "Physics/LyraCollisionChannels.h"


UPBMeleeWeaponTraceComponent::UPBMeleeWeaponTraceComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TraceChannel = Lyra_TraceChannel_Weapon;
}

void UPBMeleeWeaponTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	//RamaMeleeWeapon_OnHit.AddDynamic(this, &UPBMeleeWeaponTraceComponent::HandleRamaEvent);
}

//bool UPBMeleeWeaponTraceComponent::MeleeSweep(FHitResult& Hit, const TArray<FTransform>& BodyPreviousPose)
//{
//	if(!BatchedHitResults.IsEmpty())
//	{
//		BatchedHitResults.Reset();
//	}
//
//	bool didHit = Super::MeleeSweep(Hit, BodyPreviousPose);
//
//	if(PBMeleeWeapon_OnHitBatched.IsBound())
//	{
//		if(BatchedHitResults.Num() > 0)
//		{
//			PBMeleeWeapon_OnHitBatched.Broadcast(BatchedHitResults);
//		}
//	}
//
//	BatchedHitResults.Reset();
//
//	return didHit;
//}

bool UPBMeleeWeaponTraceComponent::MeleeSweep(FHitResult& Hit, const TArray<FTransform>& BodyPreviousPose)
{
	AActor* Actor = GetOwner();
	FBodyInstance* RootBody = GetBodyInstance();
	if (!RootBody) return false;

	//RamaMeleeShape.h for definition of FMeleeSweepData
	TArray<FMeleeSweepData> DamageShapesData;

	URamaMeleeCore::GetMeleeSweepData(
		this,
		BodyPreviousPose,
		DamageShapesData,
		DamageMap
	);

	//if(WITH_EDITOR) UE_LOG(LogTemp,Warning,TEXT("dmg shape count %d"), DamageShapesData.Num());


	//Do All Damage Shapes First!

	TArray<FHitResult> BatchedHitResults;

	bool AnyHitOccurred = false;
	for (FMeleeSweepData& EachMeleeSweep : DamageShapesData) //Damage
	{
		if (DrawSweeps)
		{
			EachMeleeSweep.DrawStart(
				GetWorld(),
				DrawShapes_Thickness,
				FColor::Red,
				DrawShapes_Duration
			);
			EachMeleeSweep.DrawEnd(
				GetWorld(),
				DrawShapes_Thickness,
				FColor::Red,
				DrawShapes_Duration
			);
		}

		//Sweep as Damage
		if (PerformDeepTrace)
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(Actor);
			int32 Iterations = 0;
			int32 MaxIterations = 10000;
			while (Iterations < MaxIterations)
			{
				if (Iterations >= MaxIterations - 1)
				{
					UE_LOG(LogTemp, Warning, TEXT("Melee Weapon Infinite Loop Occurred!"));
					return true;
				}

				if (UPBMeleeCore::MeleeSweepByChannel(
					this,
					GetWorld(),
					ActorsToIgnore,
					Hit,
					EachMeleeSweep,
					TraceChannel
				)) {
					//BroadCast
					if (RamaMeleeWeapon_OnHit.IsBound())
					{
						RamaMeleeWeapon_OnHit.Broadcast(Hit.GetActor(), Hit.GetComponent(), Hit.ImpactPoint, Hit.ImpactNormal, EachMeleeSweep.ShapeIndex, Hit.BoneName, Hit);
					}

					BatchedHitResults.Add(Hit);

					//Perform the trace again, ignoring the hit actor, to find ALL _unique_ actors along same path.
					ActorsToIgnore.Add(Hit.GetActor());
					Iterations++;
					AnyHitOccurred = true;
					continue;
				}

				//No hit occurred this time, done!

				

				//Performance
				if (DeepTrace_SingleBodyMode)
				{
					if (PBMeleeWeapon_OnHitBatched.IsBound())
					{
						if (BatchedHitResults.Num() > 0)
						{
							PBMeleeWeapon_OnHitBatched.Broadcast(BatchedHitResults);
						}
					}

					return AnyHitOccurred;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			if (UPBMeleeCore::MeleeSweepByChannel(
				this,
				GetWorld(),
				Actor,
				Hit,
				EachMeleeSweep,
				TraceChannel
			)) {
				//BroadCast
				if (RamaMeleeWeapon_OnHit.IsBound())
				{
					RamaMeleeWeapon_OnHit.Broadcast(Hit.GetActor(), Hit.GetComponent(), Hit.ImpactPoint, Hit.ImpactNormal, EachMeleeSweep.ShapeIndex, Hit.BoneName, Hit);
				}

				if (PBMeleeWeapon_OnHitBatched.IsBound())
				{
					BatchedHitResults.Add(Hit);
					PBMeleeWeapon_OnHitBatched.Broadcast(BatchedHitResults);
				}

				return true;
				//~~~~ Save performance, we already hit something
			}
		}
	}

	if (PBMeleeWeapon_OnHitBatched.IsBound())
	{
		if (BatchedHitResults.Num() > 0)
		{
			PBMeleeWeapon_OnHitBatched.Broadcast(BatchedHitResults);
		}
	}

	return AnyHitOccurred;
}

