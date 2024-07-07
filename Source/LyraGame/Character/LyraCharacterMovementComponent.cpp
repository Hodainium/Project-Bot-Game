// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "ProjectBotSource/Tags/PB_Tags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCharacterMovementComponent)

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_MovementStopped, "Gameplay.MovementStopped");

namespace LyraCharacter
{
	static float GroundTraceDistance = 100000.0f;
	FAutoConsoleVariableRef CVar_GroundTraceDistance(TEXT("LyraCharacter.GroundTraceDistance"), GroundTraceDistance, TEXT("Distance to trace down when generating ground information."), ECVF_Cheat);
};

ULyraCharacterMovementComponent::ULyraCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxWalkSpeedSprinting = 600.f;
}

void ULyraCharacterMovementComponent::SimulateMovement(float DeltaTime)
{
	if (bHasReplicatedAcceleration)
	{
		// Preserve our replicated acceleration
		const FVector OriginalAcceleration = Acceleration;
		Super::SimulateMovement(DeltaTime);
		Acceleration = OriginalAcceleration;
	}
	else
	{
		Super::SimulateMovement(DeltaTime);
	}
}

bool ULyraCharacterMovementComponent::CanAttemptJump() const
{
	// Same as UCharacterMovementComponent's implementation but without the crouch check
	return IsJumpAllowed() &&
		(IsMovingOnGround() || IsFalling()); // Falling included for double-jump and non-zero jump hold time, but validated by character.
}

void ULyraCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

const FLyraCharacterGroundInfo& ULyraCharacterMovementComponent::GetGroundInfo()
{
	if (!CharacterOwner || (GFrameCounter == CachedGroundInfo.LastUpdateFrame))
	{
		return CachedGroundInfo;
	}

	if (MovementMode == MOVE_Walking)
	{
		CachedGroundInfo.GroundHitResult = CurrentFloor.HitResult;
		CachedGroundInfo.GroundDistance = 0.0f;
	}
	else
	{
		const UCapsuleComponent* CapsuleComp = CharacterOwner->GetCapsuleComponent();
		check(CapsuleComp);

		const float CapsuleHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
		const ECollisionChannel CollisionChannel = (UpdatedComponent ? UpdatedComponent->GetCollisionObjectType() : ECC_Pawn);
		const FVector TraceStart(GetActorLocation());
		const FVector TraceEnd(TraceStart.X, TraceStart.Y, (TraceStart.Z - LyraCharacter::GroundTraceDistance - CapsuleHalfHeight));

		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(LyraCharacterMovementComponent_GetGroundInfo), false, CharacterOwner);
		FCollisionResponseParams ResponseParam;
		InitCollisionParams(QueryParams, ResponseParam);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParams, ResponseParam);

		CachedGroundInfo.GroundHitResult = HitResult;
		CachedGroundInfo.GroundDistance = LyraCharacter::GroundTraceDistance;

		if (MovementMode == MOVE_NavWalking)
		{
			CachedGroundInfo.GroundDistance = 0.0f;
		}
		else if (HitResult.bBlockingHit)
		{
			CachedGroundInfo.GroundDistance = FMath::Max((HitResult.Distance - CapsuleHalfHeight), 0.0f);
		}
	}

	CachedGroundInfo.LastUpdateFrame = GFrameCounter;

	return CachedGroundInfo;
}

void ULyraCharacterMovementComponent::SetReplicatedAcceleration(const FVector& InAcceleration)
{
	bHasReplicatedAcceleration = true;
	Acceleration = InAcceleration;
}

FRotator ULyraCharacterMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if (ASC->HasMatchingGameplayTag(TAG_Gameplay_MovementStopped))
		{
			return FRotator(0,0,0);
		}

		if (ASC->HasMatchingGameplayTag(PB_MovementSetting_Tags::TAG_MOVEMENT_SETTING_LOCKROTATION))
		{
			return FRotator(0, 0, 0);
		}
	}

	return Super::GetDeltaRotation(DeltaTime);
}

float ULyraCharacterMovementComponent::GetMaxSpeed() const
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if (ASC->HasMatchingGameplayTag(TAG_Gameplay_MovementStopped))
		{
			return 0;
		}

		if (ASC->HasMatchingGameplayTag(PB_MovementSetting_Tags::TAG_MOVEMENT_SETTING_LOCKMOVEMENT))
		{
			return 0;
		}
	}

	if (IsSprinting())
	{
		return MaxWalkSpeedSprinting;
	}

	return Super::GetMaxSpeed();
}

float ULyraCharacterMovementComponent::GetMaxAcceleration() const
{
	/*if (IsSprinting() && (!bUseMaxAccelerationSprintingOnlyAtSpeed || IsSprintingAtSpeed()))
	{
		return MaxAccelerationSprinting;
	}*/

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if (ASC->HasMatchingGameplayTag(TAG_Gameplay_MovementStopped))
		{
			return 0;
		}

		if (ASC->HasMatchingGameplayTag(PB_MovementSetting_Tags::TAG_MOVEMENT_SETTING_LOCKMOVEMENT))
		{
			return 0;
		}
	}

	return Super::GetMaxAcceleration();
}

void ULyraCharacterMovementComponent::FPBSavedMove::Clear()
{
	Super::Clear();

	SavedWantsToSprint = false;
}

uint8 ULyraCharacterMovementComponent::FPBSavedMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

bool ULyraCharacterMovementComponent::FPBSavedMove::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (SavedWantsToSprint != ((FPBSavedMove*)&NewMove)->SavedWantsToSprint)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void ULyraCharacterMovementComponent::FPBSavedMove::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	ULyraCharacterMovementComponent* CharacterMovement = Cast<ULyraCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		SavedWantsToSprint = CharacterMovement->bWantsToSprint;
	}
}

void ULyraCharacterMovementComponent::FPBSavedMove::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	ULyraCharacterMovementComponent* CharacterMovement = Cast<ULyraCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
	}
}

ULyraCharacterMovementComponent::FPBNetworkPredictionData_Client::FPBNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr ULyraCharacterMovementComponent::FPBNetworkPredictionData_Client::AllocateNewMove()
{
	return MakeShared<FPBSavedMove>();
}

FNetworkPredictionData_Client* ULyraCharacterMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		ULyraCharacterMovementComponent* MutableThis = const_cast<ULyraCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FPBNetworkPredictionData_Client(*this);
	}

	return ClientPredictionData;
}

void ULyraCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	//The Flags parameter contains the compressed input flags that are stored in the saved move.
	//UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	//It basically just resets the movement component to the state when the move was made so it can simulate from there.
	bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void ULyraCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	// Proxies get replicated Sprint state.
	if (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)
	{
		// Check for a change in Sprint state. Players toggle Sprint by changing bWantsToSprint.
		const bool bIsSprinting = IsSprinting();
		if (bIsSprinting && (!bWantsToSprint || !CanSprintInCurrentState()))
		{
			UnSprint(false);
		}
		else if (!bIsSprinting && bWantsToSprint && CanSprintInCurrentState())
		{
			Sprint(false);
		}
	}

	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void ULyraCharacterMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	// Proxies get replicated Sprint state.
	if (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)
	{
		// UnSprint if no longer allowed to be Sprinting
		if (IsSprinting() && !CanSprintInCurrentState())
		{
			UnSprint(false);
		}
	}

	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);
}

bool ULyraCharacterMovementComponent::IsSprinting() const
{
	return LyraCharacterOwner && LyraCharacterOwner->bIsSprinting;
}

void ULyraCharacterMovementComponent::Sprint(bool bClientSimulation)
{
	if (!HasValidData())
	{
		return;
	}

	if (!bClientSimulation && !CanSprintInCurrentState())
	{
		return;
	}

	if (!bClientSimulation)
	{
		LyraCharacterOwner->bIsSprinting = true;
	}
	LyraCharacterOwner->OnStartSprint();
}

void ULyraCharacterMovementComponent::UnSprint(bool bClientSimulation)
{
	if (!HasValidData())
	{
		return;
	}

	if (!bClientSimulation)
	{
		LyraCharacterOwner->bIsSprinting = false;
	}
	LyraCharacterOwner->OnEndSprint();
}

bool ULyraCharacterMovementComponent::ClientUpdatePositionAfterServerUpdate()
{
	const bool bRealSprint = bWantsToSprint;
	const bool bResult = Super::ClientUpdatePositionAfterServerUpdate();
	bWantsToSprint = bRealSprint;

	return bResult;
}

bool ULyraCharacterMovementComponent::CanSprintInCurrentState() const
{
	/*if (!UpdatedComponent || UpdatedComponent->IsSimulatingPhysics())
	{
		return false;
	}

	if (!IsFalling() && !IsMovingOnGround())
	{
		return false;
	}*/

	/*if (!IsSprintWithinAllowableInputAngle())
	{
		return false;
	}*/

	return true;
}

bool ULyraCharacterMovementComponent::HasValidData() const
{
	return Super::HasValidData() && IsValid(LyraCharacterOwner);
}

void ULyraCharacterMovementComponent::PostLoad()
{
	Super::PostLoad();

	LyraCharacterOwner = Cast<ALyraCharacter>(PawnOwner);
}

void ULyraCharacterMovementComponent::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	Super::SetUpdatedComponent(NewUpdatedComponent);

	LyraCharacterOwner = Cast<ALyraCharacter>(PawnOwner);
}
