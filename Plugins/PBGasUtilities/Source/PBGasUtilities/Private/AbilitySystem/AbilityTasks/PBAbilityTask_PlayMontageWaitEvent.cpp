// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/AbilityTasks/PBAbilityTask_PlayMontageWaitEvent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemLog.h"
#include "GameFramework/Character.h"

static bool GPBUseAggressivePlayMontageAndWaitEventEndTask = true;
static FAutoConsoleVariableRef CVarAggressivePlayMontageAndWaitEventEndTask(TEXT("AbilitySystem.PBPlayMontageWaitEvent.AggressiveEndTask"), GPBUseAggressivePlayMontageAndWaitEventEndTask,
                                                                            TEXT("This should be set to true in order to avoid multiple callbacks off an PBAbilityTask_PlayMontageWaitEvent node"));

void UPBAbilityTask_PlayMontageWaitEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	const bool bPlayingThisMontage = (Montage == MontageToPlay) && Ability && Ability->GetCurrentMontage() == MontageToPlay;
	if (bPlayingThisMontage)
	{
		// Reset AnimRootMotionTranslationScale
		ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
		if (Character && (Character->GetLocalRole() == ROLE_Authority ||
			(Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
		{
			Character->SetAnimRootMotionTranslationScale(1.f);
		}
	}

	if (bPlayingThisMontage && (bInterrupted || !bAllowInterruptAfterBlendOut))
	{
		if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
		{
			ASC->ClearAnimatingAbility(Ability);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (bInterrupted)
		{
			OnInterrupted.Broadcast(FGameplayTag::EmptyTag, FGameplayEventData());

			if (GPBUseAggressivePlayMontageAndWaitEventEndTask)
			{
				EndTask();
			}
		}
		else
		{
			OnBlendOut.Broadcast(FGameplayTag::EmptyTag, FGameplayEventData());
		}
	}
}

void UPBAbilityTask_PlayMontageWaitEvent::OnGameplayAbilityCancelled()
{
	if (StopPlayingMontage() || bAllowInterruptAfterBlendOut)
	{
		// Let the BP handle the interrupt as well
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.Broadcast(FGameplayTag::EmptyTag, FGameplayEventData());
		}
	}

	if (GPBUseAggressivePlayMontageAndWaitEventEndTask)
	{
		EndTask();
	}
}

void UPBAbilityTask_PlayMontageWaitEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleted.Broadcast(FGameplayTag::EmptyTag, FGameplayEventData());
		}
	}

	EndTask();
}

UPBAbilityTask_PlayMontageWaitEvent* UPBAbilityTask_PlayMontageWaitEvent::CreatePlayMontageAndWaitProxyTags(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay,
                                                                                                                FGameplayTagContainer EventTags, float Rate, FName StartSection, bool bStopWhenAbilityEnds,
                                                                                                                float AnimRootMotionTranslationScale, float StartTimeSeconds, bool bAllowInterruptAfterBlendOut, bool bOnlyExact)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UPBAbilityTask_PlayMontageWaitEvent* MyObj = NewAbilityTask<UPBAbilityTask_PlayMontageWaitEvent>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->bAllowInterruptAfterBlendOut = bAllowInterruptAfterBlendOut;
	MyObj->StartTimeSeconds = StartTimeSeconds;
	MyObj->EventTags = EventTags;
	MyObj->bOnlyExact = bOnlyExact;

	return MyObj;
}

UPBAbilityTask_PlayMontageWaitEvent* UPBAbilityTask_PlayMontageWaitEvent::CreatePlayMontageAndWaitProxy(UGameplayAbility* OwningAbility,
                                                                                                            FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTag EventTag, float Rate, FName StartSection,
                                                                                                            bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale, float StartTimeSeconds, bool bAllowInterruptAfterBlendOut,
                                                                                                            bool bOnlyExact)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UPBAbilityTask_PlayMontageWaitEvent* MyObj = NewAbilityTask<UPBAbilityTask_PlayMontageWaitEvent>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->bAllowInterruptAfterBlendOut = bAllowInterruptAfterBlendOut;
	MyObj->StartTimeSeconds = StartTimeSeconds;
	MyObj->EventTags = FGameplayTagContainer(EventTag);
	MyObj->bOnlyExact = bOnlyExact;

	return MyObj;
}

void UPBAbilityTask_PlayMontageWaitEvent::GameplayEventContainerCallback(FGameplayTag GameplayTag, const FGameplayEventData* GameplayEventData)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (const bool bValid = bOnlyExact ? EventTags.HasTagExact(GameplayTag) : EventTags.HasTag(GameplayTag))
		{
			FGameplayEventData TempPayload = *GameplayEventData;
			TempPayload.EventTag = GameplayTag;
			OnEvent.Broadcast(GameplayTag, MoveTemp(TempPayload));
		}
	}
}

void UPBAbilityTask_PlayMontageWaitEvent::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	bool bPlayedMontage = false;

	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			if (ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection, StartTimeSeconds) > 0.f)
			{
				// Playing a montage could potentially fire off a callback into game code which could kill this ability! Early out if we are  pending kill.
				if (ShouldBroadcastAbilityTaskDelegates() == false)
				{
					return;
				}

				SingleHandle = ASC->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UPBAbilityTask_PlayMontageWaitEvent::GameplayEventContainerCallback));

				InterruptedHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UPBAbilityTask_PlayMontageWaitEvent::OnGameplayAbilityCancelled);

				BlendingOutDelegate.BindUObject(this, &UPBAbilityTask_PlayMontageWaitEvent::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UPBAbilityTask_PlayMontageWaitEvent::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
				if (Character && (Character->GetLocalRole() == ROLE_Authority ||
					(Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
				{
					Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
				}

				bPlayedMontage = true;
			}
		}
		else
		{
			ABILITY_LOG(Warning, TEXT("UPBAbilityTask_PlayMontageWaitEvent call to PlayMontage failed!"));
		}
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("UPBAbilityTask_PlayMontageWaitEvent called on invalid AbilitySystemComponent"));
	}

	if (!bPlayedMontage)
	{
		ABILITY_LOG(Warning, TEXT("UPBAbilityTask_PlayMontageWaitEvent called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay), *InstanceName.ToString());
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag::EmptyTag, FGameplayEventData());
		}
	}

	SetWaitingOnAvatar();
}

void UPBAbilityTask_PlayMontageWaitEvent::ExternalCancel()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCancelled.Broadcast(FGameplayTag::EmptyTag, FGameplayEventData());
	}
	Super::ExternalCancel();
}

void UPBAbilityTask_PlayMontageWaitEvent::OnDestroy(bool AbilityEnded)
{
	// Note: Clearing montage end delegate isn't necessary since its not a multicast and will be cleared when the next montage plays.
	// (If we are destroyed, it will detect this and not do anything)

	// This delegate, however, should be cleared as it is a multicast
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(InterruptedHandle);
		if (AbilityEnded && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}

	//Remove our event binding
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		ASC->RemoveGameplayEventTagContainerDelegate(EventTags, SingleHandle);
	}

	Super::OnDestroy(AbilityEnded);
}

bool UPBAbilityTask_PlayMontageWaitEvent::StopPlayingMontage()
{
	if (Ability == nullptr)
	{
		return false;
	}

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (ActorInfo == nullptr)
	{
		return false;
	}

	const UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}

	// Check if the montage is still playing
	// The ability would have been interrupted, in which case we should automatically stop the montage
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && Ability)
	{
		if (ASC->GetAnimatingAbility() == Ability
			&& ASC->GetCurrentMontage() == MontageToPlay)
		{
			// Unbind delegates so they don't get called as well
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			ASC->CurrentMontageStop();
			return true;
		}
	}

	return false;
}

FString UPBAbilityTask_PlayMontageWaitEvent::GetDebugString() const
{
	const UAnimMontage* PlayingMontage = nullptr;
	if (Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		const UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? ToRawPtr(MontageToPlay) : AnimInstance->GetCurrentActiveMontage();
		}
	}

	return FString::Printf(TEXT("PlayMontageAndWait. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}
