// Fill out your copyright notice in the Description page of Project Settings.

// Copyright Epic Games, Inc. All Rights Reserved.

#include "PBGameplayAbility_MeleeWeapon.h"

#include "PBGameplayAbility_RangedWeapon.h"

#include "Physics/LyraCollisionChannels.h"
#include "AIController.h"
#include "NativeGameplayTags.h"
#include "Weapons/LyraWeaponStateComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/LyraGameplayAbilityTargetData_SingleTargetHit.h"
#include "DrawDebugHelpers.h"
#include "PBMeleeWeaponInstance.h"
#include "PBMeleeWeaponTraceComponent.h"
#include "PBRangedWeaponInstance.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"
#include "RamaMeleeWeapon.h"
#include "Logging/StructuredLog.h"

// Weapon fire will be blocked/canceled if the player has this tag
//UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_PB_WeaponFireBlocked, "Ability.Weapon.NoFiring");


UPBGameplayAbility_MeleeWeapon::UPBGameplayAbility_MeleeWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//SourceBlockedTags.AddTag(TAG_PB_WeaponFireBlocked);
	//TargettingSource = EPBAbilityTargetingSource::CameraTowardsFocus;
	IsTracing = false;
}

UPBMeleeWeaponInstance* UPBGameplayAbility_MeleeWeapon::GetMeleeWeaponInstance() const
{
	return Cast<UPBMeleeWeaponInstance>(GetAssociatedEquipment());
}

bool UPBGameplayAbility_MeleeWeapon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	if (bResult)
	{
		if (GetMeleeWeaponInstance() == nullptr)
		{
			UE_LOG(LogPBAbilitySystem, Error, TEXT("Weapon ability %s cannot be activated because there is no associated ranged weapon (equipment instance=%s but needs to be derived from %s)"),
				*GetPathName(),
				*GetPathNameSafe(GetAssociatedEquipment()),
				*UPBRangedWeaponInstance::StaticClass()->GetName());
			bResult = false;
		}
	}

	return bResult;
}

void UPBGameplayAbility_MeleeWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Bind target data callback
	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);

	OnTargetDataReadyCallbackDelegateHandle = MyAbilityComponent->AbilityTargetDataSetDelegate(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).AddUObject(this, &ThisClass::OnTargetDataReadyCallback);

	// Update the last firing time
	UPBMeleeWeaponInstance* WeaponData = GetMeleeWeaponInstance();
	check(WeaponData);
	WeaponData->UpdateFiringTime();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UPBGameplayAbility_MeleeWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsEndAbilityValid(Handle, ActorInfo))
	{
		if (ScopeLockCount > 0)
		{
			WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &ThisClass::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
			return;
		}

		UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
		check(MyAbilityComponent);

		// When ability ends, consume target data and remove delegate
		MyAbilityComponent->AbilityTargetDataSetDelegate(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).Remove(OnTargetDataReadyCallbackDelegateHandle);
		MyAbilityComponent->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

void UPBGameplayAbility_MeleeWeapon::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);

	if (const FGameplayAbilitySpec* AbilitySpec = MyAbilityComponent->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		FScopedPredictionWindow	ScopedPrediction(MyAbilityComponent);

		// Take ownership of the target data to make sure no callbacks into game code invalidate it out from under us
		FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));

		const bool bShouldNotifyServer = CurrentActorInfo->IsLocallyControlled() && !CurrentActorInfo->IsNetAuthority();
		if (bShouldNotifyServer)
		{
			MyAbilityComponent->CallServerSetReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey(), LocalTargetDataHandle, ApplicationTag, MyAbilityComponent->ScopedPredictionKey);
		}

		const bool bIsTargetDataValid = true;

#if WITH_SERVER_CODE
		if (AController* Controller = GetControllerFromActorInfo())
		{
			if (Controller->GetLocalRole() == ROLE_Authority)
			{
				// Confirm hit markers
				if (ULyraWeaponStateComponent* WeaponStateComponent = Controller->FindComponentByClass<ULyraWeaponStateComponent>())
				{
					TArray<uint8> HitReplaces;
					for (uint8 i = 0; (i < LocalTargetDataHandle.Num()) && (i < 255); ++i)
					{
						if (FGameplayAbilityTargetData_SingleTargetHit* SingleTargetHit = static_cast<FGameplayAbilityTargetData_SingleTargetHit*>(LocalTargetDataHandle.Get(i)))
						{
							if (SingleTargetHit->bHitReplaced)
							{
								HitReplaces.Add(i);
							}
						}
					}

					WeaponStateComponent->ClientConfirmTargetData(LocalTargetDataHandle.UniqueId, bIsTargetDataValid, HitReplaces);
				}

			}
		}
#endif //WITH_SERVER_CODE


		// See if we still have ammo
		if (bIsTargetDataValid && CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
		{
			// We fired the weapon, add spread
			UPBMeleeWeaponInstance* WeaponData = GetMeleeWeaponInstance();
			check(WeaponData);
			//WeaponData->AddSpread();

			// Let the blueprint do stuff like apply effects to the targets
			OnRangedWeaponTargetDataReady(LocalTargetDataHandle);
		}
		else
		{
			UE_LOG(LogPBAbilitySystem, Warning, TEXT("Weapon ability %s failed to commit (bIsTargetDataValid=%d)"), *GetPathName(), bIsTargetDataValid ? 1 : 0);
			K2_EndAbility();
		}
	}

	// We've processed the data
	MyAbilityComponent->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
}

void UPBGameplayAbility_MeleeWeapon::StartMeleeWeaponTracing()
{
	//TODO cache the melee comps so we dont have to search and loop twice

	UE_LOGFMT(LogPBGame, Warning, "Enabled melee trace1");

	UPBMeleeWeaponInstance* Instance = GetMeleeWeaponInstance();

	if(!Instance)
	{
		return;
	}

	TArray<AActor*> SpawnedActors = Instance->GetSpawnedActors();

	if(IsLocallyControlled())
	{
		UE_LOGFMT(LogPBGame, Warning, "Enabled melee trace2, size: {num}", SpawnedActors.Num());

		for (AActor* Actor : SpawnedActors)
		{
			if (UPBMeleeWeaponTraceComponent* MeleeComp = Actor->GetComponentByClass<UPBMeleeWeaponTraceComponent>())
			{
				MeleeComp->StartSwingDamage();
				IsTracing = true;
				UE_LOGFMT(LogPBGame, Warning, "Enabled melee trace3");
				MeleeComp->PBMeleeWeapon_OnHitBatched.AddDynamic(this, &ThisClass::OnWeaponTickFinished);
			}
		}
	}
}

void UPBGameplayAbility_MeleeWeapon::StopMeleeWeaponTracing()
{
	//TODO cache the melee comps so we dont have to search and loop twice

	UE_LOGFMT(LogPBGame, Warning, "Disabled melee trace1");

	UPBMeleeWeaponInstance* Instance = GetMeleeWeaponInstance();

	if (!Instance)
	{
		return;
	}

	TArray<AActor*> SpawnedActors = Instance->GetSpawnedActors();

	if (IsLocallyControlled() && IsTracing)
	{
		UE_LOGFMT(LogPBGame, Warning, "Disabled melee trace2");

		for (AActor* Actor : SpawnedActors)
		{
			if (UPBMeleeWeaponTraceComponent* MeleeComp = Actor->GetComponentByClass<UPBMeleeWeaponTraceComponent>())
			{
				MeleeComp->StopSwingDamage();
				IsTracing = false;
				UE_LOGFMT(LogPBGame, Warning, "Disabled melee trace3");
				MeleeComp->PBMeleeWeapon_OnHitBatched.RemoveDynamic(this, &ThisClass::OnWeaponTickFinished);
			}
		}
	}
}

void UPBGameplayAbility_MeleeWeapon::OnWeaponTickFinished(const TArray<FHitResult>& InHits)
{
	check(CurrentActorInfo);

	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
	check(AvatarActor);

	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);

	AController* Controller = GetControllerFromActorInfo();
	check(Controller);
	ULyraWeaponStateComponent* WeaponStateComponent = Controller->FindComponentByClass<ULyraWeaponStateComponent>();

	FScopedPredictionWindow ScopedPrediction(MyAbilityComponent, CurrentActivationInfo.GetActivationPredictionKey());

	// Fill out the target data from the hit results
	FGameplayAbilityTargetDataHandle TargetData;
	TargetData.UniqueId = WeaponStateComponent ? WeaponStateComponent->GetUnconfirmedServerSideHitMarkerCount() : 0;

	if (InHits.Num() > 0)
	{
		const int32 CartridgeID = FMath::Rand();

		for (const FHitResult& FoundHit : InHits)
		{
			FLyraGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FLyraGameplayAbilityTargetData_SingleTargetHit();
			NewTargetData->HitResult = FoundHit;
			NewTargetData->CartridgeID = CartridgeID;

			TargetData.Add(NewTargetData);
		}
	}

	// Send hit marker information
	if (WeaponStateComponent != nullptr)
	{
		WeaponStateComponent->AddUnconfirmedServerSideHitMarkers(TargetData, InHits);
	}

	// Process the target data immediately
	OnTargetDataReadyCallback(TargetData, FGameplayTag());
}