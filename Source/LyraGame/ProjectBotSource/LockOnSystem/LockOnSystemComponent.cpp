// Fill out your copyright notice in the Description page of Project Settings.

#include "LockOnSystemComponent.h"
#include "EngineUtils.h"
#include "TargetSystemLog.h"
#include "LockOnSystemTargetableInterface.h"
#include "LockOnSystemTargetComponent.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Logging/StructuredLog.h"
#include "Player/LyraPlayerController.h"
#include "UI/IndicatorSystem/IndicatorDescriptor.h"
#include "UI/IndicatorSystem/LyraIndicatorManagerComponent.h"

namespace PBConsoleVariables
{
	static float DrawTargetDuration = 0.0f;
	static FAutoConsoleVariableRef CVarDrawTargetDuration(
		TEXT("pb.LockOn.DrawTargetDuration"),
		DrawTargetDuration,
		TEXT("Should we do debug drawing for locked on target transform (if above zero, sets how long (in seconds))"),
		ECVF_Default);

	static float DrawTargetRadius = 3.0f;
	static FAutoConsoleVariableRef CVarDrawTargetRadius(
		TEXT("pb.LockOn.DrawTargetRadius"),
		DrawTargetRadius,
		TEXT("When locked on debug drawing is enabled (see DrawTargetDuration), how big should the hit radius be? (in uu)"),
		ECVF_Default);
}

// Sets default values for this component's properties
ULockOnSystemComponent::ULockOnSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TargetableActors = APawn::StaticClass();
	TargetableCollisionChannel = ECollisionChannel::ECC_Pawn;
}

// Called when the game starts
void ULockOnSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		TS_LOG(Error, TEXT("[%s] TargetSystemComponent: Cannot get Owner reference ..."), *GetName());
		return;
	}

	OwnerPawn = Cast<APawn>(OwnerActor);
	if (!ensure(OwnerPawn))
	{
		TS_LOG(Error, TEXT("[%s] TargetSystemComponent: Component is meant to be added to Pawn only ..."), *GetName());
		return;
	}

	SetupLocalPlayerController();
}

void ULockOnSystemComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(OwnerPawn->IsLocallyControlled())
	{
		
	}

	if (!bTargetLocked || !LockedOnTargetActor)
	{
		return;
	}

	if (!TargetIsTargetable(LockedOnTargetActor))
	{
		TargetLockOff();
		return;
	}

	SetControlRotationOnTarget(LockedOnTargetActor);

	// Target Locked Off based on Distance
	if (GetDistanceFromCharacter(LockedOnTargetActor) > MinimumDistanceToEnable)
	{
		TargetLockOff();
	}

	if (ShouldBreakLineOfSight() && !bIsBreakingLineOfSight)
	{
		if (BreakLineOfSightDelay <= 0)
		{
			TargetLockOff();
		}
		else
		{
			bIsBreakingLineOfSight = true;
			GetWorld()->GetTimerManager().SetTimer(
				LineOfSightBreakTimerHandle,
				this,
				&ULockOnSystemComponent::BreakLineOfSight,
				BreakLineOfSightDelay
			);
		}
	}
}

void ULockOnSystemComponent::TargetActor()
{
	ClosestTargetDistance = MinimumDistanceToEnable;

	if (bTargetLocked)
	{
		TargetLockOff();
	}
	else
	{
		const TArray<AActor*> Actors = GetAllActorsOfClass(TargetableActors);
		LockedOnTargetActor = FindNearestTarget(Actors);
		TargetLockOn(LockedOnTargetActor);
	}
}

void ULockOnSystemComponent::TargetActorWithAxisInput(const float AxisValue)
{
	// If we're not locked on, do nothing
	if (!bTargetLocked)
	{
		return;
	}

	if (!LockedOnTargetActor)
	{
		return;
	}

	// If we're not allowed to switch target, do nothing
	if (!ShouldSwitchTargetActor(AxisValue))
	{
		return;
	}

	// If we're switching target, do nothing for a set amount of time
	if (bIsSwitchingTarget)
	{
		return;
	}

	// Lock off target
	AActor* CurrentTarget = LockedOnTargetActor;

	// Depending on Axis Value negative / positive, set Direction to Look for (negative: left, positive: right)
	const float RangeMin = AxisValue < 0 ? 0 : 180;
	const float RangeMax = AxisValue < 0 ? 180 : 360;

	// Reset Closest Target Distance to Minimum Distance to Enable
	ClosestTargetDistance = MinimumDistanceToEnable;

	// Get All Actors of Class
	TArray<AActor*> Actors = GetAllActorsOfClass(TargetableActors);

	// For each of these actors, check line trace and ignore Current Target and build the list of actors to look from
	TArray<AActor*> ActorsToLook;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(CurrentTarget);
	for (AActor* Actor : Actors)
	{
		const bool bHit = LineTraceForActor(Actor, ActorsToIgnore);
		if (bHit && IsInViewport(Actor))
		{
			ActorsToLook.Add(Actor);
		}
	}

	// Find Targets in Range (left or right, based on Character and CurrentTarget)
	TArray<AActor*> TargetsInRange = FindTargetsInRange(ActorsToLook, RangeMin, RangeMax);

	// For each of these targets in range, get the closest one to current target
	AActor* ActorToTarget = nullptr;
	for (AActor* Actor : TargetsInRange)
	{
		// and filter out any character too distant from minimum distance to enable
		const float Distance = GetDistanceFromCharacter(Actor);
		if (Distance < MinimumDistanceToEnable)
		{
			const float RelativeActorsDistance = CurrentTarget->GetDistanceTo(Actor);
			if (RelativeActorsDistance < ClosestTargetDistance)
			{
				ClosestTargetDistance = RelativeActorsDistance;
				ActorToTarget = Actor;
			}
		}
	}

	if (ActorToTarget)
	{
		if (SwitchingTargetTimerHandle.IsValid())
		{
			SwitchingTargetTimerHandle.Invalidate();
		}

		TargetLockOff();
		LockedOnTargetActor = ActorToTarget;
		TargetLockOn(ActorToTarget);

		GetWorld()->GetTimerManager().SetTimer(
			SwitchingTargetTimerHandle,
			this,
			&ULockOnSystemComponent::ResetIsSwitchingTarget,
			// Less sticky if still switching
			bIsSwitchingTarget ? 0.25f : 0.5f
		);

		bIsSwitchingTarget = true;
	}
}

bool ULockOnSystemComponent::GetTargetLockedStatus()
{
	return bTargetLocked;
}

AActor* ULockOnSystemComponent::GetLockedOnTargetActor() const
{
	return LockedOnTargetActor;
}

bool ULockOnSystemComponent::IsLocked() const
{
	return bTargetLocked && LockedOnTargetActor;
}

void ULockOnSystemComponent::SetOwnerPawnLockOnState(bool State)
{
	if (bShouldControlRotation)
	{
		ControlRotation(State);
	}

	if (IsValid(OwnerPlayerController))
	{
		if(State && (bAdjustPitchBasedOnDistanceToTarget || bIgnoreLookInput))
		{
			OwnerPlayerController->SetIgnoreLookInput(true);
		}
		else
		{
			OwnerPlayerController->ResetIgnoreLookInput();
		}
	}
}

TArray<AActor*> ULockOnSystemComponent::FindTargetsInRange(TArray<AActor*> ActorsToLook, const float RangeMin, const float RangeMax) const
{
	TArray<AActor*> ActorsInRange;

	for (AActor* Actor : ActorsToLook)
	{
		const float Angle = GetAngleUsingCameraRotation(Actor);
		if (Angle > RangeMin && Angle < RangeMax)
		{
			ActorsInRange.Add(Actor);
		}
	}

	return ActorsInRange;
}

float ULockOnSystemComponent::GetAngleUsingCameraRotation(const AActor* ActorToLook) const
{
	UCameraComponent* CameraComponent = OwnerActor->FindComponentByClass<UCameraComponent>();
	if (!CameraComponent)
	{
		// Fallback to CharacterRotation if no CameraComponent can be found
		return GetAngleUsingCharacterRotation(ActorToLook);
	}

	const FRotator CameraWorldRotation = CameraComponent->GetComponentRotation();
	const FRotator LookAtRotation = FindLookAtRotation(CameraComponent->GetComponentLocation(), ActorToLook->GetActorLocation());

	float YawAngle = CameraWorldRotation.Yaw - LookAtRotation.Yaw;
	if (YawAngle < 0)
	{
		YawAngle = YawAngle + 360;
	}

	return YawAngle;
}

float ULockOnSystemComponent::GetAngleUsingCharacterRotation(const AActor* ActorToLook) const
{
	const FRotator CharacterRotation = OwnerActor->GetActorRotation();
	const FRotator LookAtRotation = FindLookAtRotation(OwnerActor->GetActorLocation(), ActorToLook->GetActorLocation());

	float YawAngle = CharacterRotation.Yaw - LookAtRotation.Yaw;
	if (YawAngle < 0)
	{
		YawAngle = YawAngle + 360;
	}

	return YawAngle;
}

FRotator ULockOnSystemComponent::FindLookAtRotation(const FVector Start, const FVector Target)
{
	return FRotationMatrix::MakeFromX(Target - Start).Rotator();
}

void ULockOnSystemComponent::ResetIsSwitchingTarget()
{
	bIsSwitchingTarget = false;
	bDesireToSwitch = false;
}

bool ULockOnSystemComponent::ShouldSwitchTargetActor(const float AxisValue)
{
	// Sticky feeling computation
	if (bEnableStickyTarget)
	{
		StartRotatingStack += (AxisValue != 0) ? AxisValue * AxisMultiplier : (StartRotatingStack > 0 ? -AxisMultiplier : AxisMultiplier);

		if (AxisValue == 0 && FMath::Abs(StartRotatingStack) <= AxisMultiplier)
		{
			StartRotatingStack = 0.0f;
		}

		// If Axis value does not exceeds configured threshold, do nothing
		if (FMath::Abs(StartRotatingStack) < StickyRotationThreshold)
		{
			bDesireToSwitch = false;
			return false;
		}

		//Sticky when switching target.
		if (StartRotatingStack * AxisValue > 0)
		{
			StartRotatingStack = StartRotatingStack > 0 ? StickyRotationThreshold : -StickyRotationThreshold;
		}
		else if (StartRotatingStack * AxisValue < 0)
		{
			StartRotatingStack = StartRotatingStack * -1.0f;
		}

		bDesireToSwitch = true;

		return true;
	}

	// Non Sticky feeling, check Axis value exceeds threshold
	return FMath::Abs(AxisValue) > StartRotatingThreshold;
}

void ULockOnSystemComponent::TargetLockOn(AActor* TargetToLockOn)
{
	if (!IsValid(TargetToLockOn))
	{
		return;
	}

	// Recast PlayerController in case it wasn't already setup on Begin Play (local split screen)
	SetupLocalPlayerController();

	bTargetLocked = true;
	if (bShouldDrawLockedOnWidget)
	{
		//CreateAndAttachTargetLockedOnWidgetComponent(TargetToLockOn);

		if (OwnerPlayerController)
		{
			if (ULyraIndicatorManagerComponent* IndicatorManager = ULyraIndicatorManagerComponent::GetComponent(OwnerPlayerController))
			{
				IndicatorManager->RemoveIndicator(CachedIndicator);
				
				CachedIndicator = nullptr;

				ULockOnSystemTargetComponent* TargetComp = TargetToLockOn->GetComponentByClass<ULockOnSystemTargetComponent>();

				UIndicatorDescriptor* Indicator = NewObject<UIndicatorDescriptor>();

				if(UMeshComponent* MeshComp = TargetToLockOn->GetComponentByClass<UMeshComponent>())
				{
					Indicator->SetSceneComponent(MeshComp);
					Indicator->SetComponentSocketName(TargetComp->GetSocketName());
				}
				else
				{
					Indicator->SetSceneComponent(TargetToLockOn->GetRootComponent());
				}

				Indicator->SetDataObject(TargetToLockOn);
				/*Indicator->SetSceneComponent(TargetToLockOn->FindComponentByClass<UMeshComponent>());
				Indicator->SetComponentSocketName(TargetComp->GetSocketName());*/
				Indicator->SetWorldPositionOffset(TargetComp->GetTargetOffset());
				Indicator->SetIndicatorClass(IndicatorWidgetClass);
				IndicatorManager->AddIndicator(Indicator);
				CachedIndicator = Indicator;
				
			}
			else
			{
				//TODO This should probably be a noisy warning.  Why are we updating widgets on a PC that can never do anything with them?
			}
		}
	}

	//SetOwnerPawnLockOnState(true);

	if (OnTargetLockedOn.IsBound())
	{
		OnTargetLockedOn.Broadcast(TargetToLockOn);
	}
}

void ULockOnSystemComponent::TargetLockOff()
{
	// Recast PlayerController in case it wasn't already setup on Begin Play (local split screen)
	SetupLocalPlayerController();

	bTargetLocked = false;
	if (CachedIndicator && OwnerPlayerController)
	{
		//TargetLockedOnWidgetComponent->DestroyComponent();
		if (ULyraIndicatorManagerComponent* IndicatorManager = ULyraIndicatorManagerComponent::GetComponent(OwnerPlayerController))
		{
			IndicatorManager->RemoveIndicator(CachedIndicator);
		}
		
		CachedIndicator = nullptr;
	}

	if (LockedOnTargetActor)
	{
		//SetOwnerPawnLockOnState(false);

		if (OnTargetLockedOff.IsBound())
		{
			OnTargetLockedOff.Broadcast(LockedOnTargetActor);
		}
	}

	LockedOnTargetActor = nullptr;
}

TArray<AActor*> ULockOnSystemComponent::GetAllActorsOfClass(const TSubclassOf<AActor> ActorClass) const
{
	TArray<AActor*> Actors;
	for (TActorIterator<AActor> ActorIterator(GetWorld(), ActorClass); ActorIterator; ++ActorIterator)
	{
		AActor* Actor = *ActorIterator;
		const bool bIsTargetable = TargetIsTargetable(Actor);
		if (bIsTargetable)
		{
			Actors.Add(Actor);
		}
	}

	return Actors;
}

bool ULockOnSystemComponent::TargetIsTargetable(const AActor* Actor)
{
	if(ULockOnSystemTargetComponent* Target = Actor->FindComponentByClass<ULockOnSystemTargetComponent>())
	{
		return Target->GetIsTargetable();
	}

	return false;

	/*const bool bIsImplemented = Actor->GetClass()->ImplementsInterface(ULockOnSystemTargetableInterface::StaticClass());
	if (bIsImplemented)
	{
		return ILockOnSystemTargetableInterface::Execute_IsTargetable(Actor);
	}

	return false;*/
}

void ULockOnSystemComponent::SetupLocalPlayerController()
{
	if (!IsValid(OwnerPawn))
	{
		TS_LOG(Error, TEXT("[%s] TargetSystemComponent: Component is meant to be added to Pawn only ..."), *GetName());
		return;
	}

	OwnerPlayerController = Cast<APlayerController>(OwnerPawn->GetController());
}

AActor* ULockOnSystemComponent::FindNearestTarget(TArray<AActor*> Actors) const
{
	TArray<AActor*> ActorsHit;

	// Find all actors we can line trace to
	for (AActor* Actor : Actors)
	{
		TArray<AActor*> ActorsToIgnore;
		const bool bHit = LineTraceForActor(Actor, ActorsToIgnore);
		if (bHit && IsInViewport(Actor))
		{
			ActorsHit.Add(Actor);
		}
	}

	// From the hit actors, check distance and return the nearest
	if (ActorsHit.Num() == 0)
	{
		return nullptr;
	}

	float ClosestDistance = ClosestTargetDistance;
	AActor* Target = nullptr;
	for (AActor* Actor : ActorsHit)
	{
		const float Distance = GetDistanceFromCharacter(Actor);
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			Target = Actor;
		}
	}

	return Target;
}


bool ULockOnSystemComponent::LineTraceForActor(const AActor* OtherActor, const TArray<AActor*>& ActorsToIgnore) const
{
	FHitResult HitResult;
	const bool bHit = LineTrace(HitResult, OtherActor, ActorsToIgnore);
	if (bHit)
	{
		const AActor* HitActor = HitResult.GetActor();
		if (HitActor == OtherActor)
		{
			return true;
		}
	}

	return false;
}

bool ULockOnSystemComponent::LineTrace(FHitResult& OutHitResult, const AActor* OtherActor, const TArray<AActor*>& ActorsToIgnore) const
{
	if (!IsValid(OwnerActor))
	{
		UE_LOG(LogPBTargetSystem, Warning, TEXT("ULockOnSystemComponent::LineTrace - Called with invalid OwnerActor: %s"), *GetNameSafe(OwnerActor))
		return false;
	}
	
	if (!IsValid(OtherActor))
	{
		UE_LOG(LogPBTargetSystem, Warning, TEXT("ULockOnSystemComponent::LineTrace - Called with invalid OtherActor: %s"), *GetNameSafe(OtherActor))
		return false;
	}
	
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Reserve(ActorsToIgnore.Num() + 1);
	IgnoredActors.Add(OwnerActor);
	IgnoredActors.Append(ActorsToIgnore);
	
	FCollisionQueryParams Params = FCollisionQueryParams(FName("LineTraceSingle"));
	Params.AddIgnoredActors(IgnoredActors);
	
	if (const UWorld* World = GetWorld(); IsValid(World))
	{
		return World->LineTraceSingleByChannel(
			OutHitResult,
			OwnerActor->GetActorLocation(),
			GetTargetActorLocation(OtherActor),
			TargetableCollisionChannel,
			Params
		);
	}

	UE_LOG(LogPBTargetSystem, Warning, TEXT("ULockOnSystemComponent::LineTrace - Called with invalid World: %s"), *GetNameSafe(GetWorld()))
	return false;
}

FRotator ULockOnSystemComponent::GetControlRotationOnTarget(const AActor* OtherActor) const
{
	if (!IsValid(OwnerPlayerController))
	{
		TS_LOG(Warning, TEXT("ULockOnSystemComponent::GetControlRotationOnTarget - OwnerPlayerController is not valid ..."))
		return FRotator::ZeroRotator;
	}

	const FRotator ControlRotation = OwnerPlayerController->GetControlRotation();

	FVector CamLoc;
	FRotator CamRot;
	OwnerPlayerController->GetPlayerViewPoint(/*out*/ CamLoc, /*out*/ CamRot);


	const FVector CharacterLocation = OwnerActor->GetActorLocation();
	const FVector OtherActorLocation = GetTargetActorLocation(OtherActor);

	// Find look at rotation
	const FRotator LookRotation = FRotationMatrix::MakeFromX(OtherActorLocation - CharacterLocation).Rotator(); //  
	const FRotator LookRotationFromCam = FRotationMatrix::MakeFromX(OtherActorLocation - CamLoc).Rotator();
	//float Pitch = LookRotation.Pitch;
	float Pitch = LookRotationFromCam.Pitch;
	FRotator TargetRotation;
	if (bAdjustPitchBasedOnDistanceToTarget)
	{
		const float DistanceToTarget = GetDistanceFromCharacter(OtherActor);
		const float PitchInRange = (DistanceToTarget * PitchDistanceCoefficient + PitchDistanceOffset) * -1.0f;
		const float PitchOffset = FMath::Clamp(PitchInRange, PitchMin, PitchMax);

		Pitch = Pitch + PitchOffset;
		TargetRotation = FRotator(Pitch, LookRotation.Yaw, ControlRotation.Roll);
	}
	else
	{
		if (bIgnoreLookInput)
		{
			TargetRotation = FRotator(Pitch, LookRotation.Yaw, ControlRotation.Roll);
		}
		else
		{
			TargetRotation = FRotator(ControlRotation.Pitch, LookRotation.Yaw, ControlRotation.Roll);
		}
	}

	return FMath::RInterpTo(ControlRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
}

void ULockOnSystemComponent::SetControlRotationOnTarget(AActor* TargetActor) const
{
	if (!IsValid(OwnerPlayerController))
	{
		return;
	}

	const FRotator ControlRotation = GetControlRotationOnTarget(TargetActor);
	if (OnTargetSetRotation.IsBound())
	{
		OnTargetSetRotation.Broadcast(TargetActor, ControlRotation);
	}
	else
	{
		OwnerPlayerController->SetControlRotation(ControlRotation);
	}
}

float ULockOnSystemComponent::GetDistanceFromCharacter(const AActor* OtherActor) const
{
	//return OwnerActor->GetDistanceTo(OtherActor);

	ULockOnSystemTargetComponent* Target = OtherActor->GetComponentByClass<ULockOnSystemTargetComponent>();

	return Target ? (OwnerActor->GetActorLocation() - Target->GetTargetLocation()).Size() : OwnerActor->GetDistanceTo(OtherActor);
}

FVector ULockOnSystemComponent::GetTargetActorLocation(const AActor* OtherActor) const
{
	ULockOnSystemTargetComponent* Target = OtherActor->GetComponentByClass<ULockOnSystemTargetComponent>();

#if ENABLE_DRAW_DEBUG
	if (PBConsoleVariables::DrawTargetDuration > 0.0f)
	{
		DrawDebugPoint(GetWorld(), Target ? Target->GetTargetLocation() : OtherActor->GetActorLocation(), PBConsoleVariables::DrawTargetRadius, FColor::Red, false, PBConsoleVariables::DrawTargetDuration, 255);
	}
#endif

	return Target ? Target->GetTargetLocation() : OtherActor->GetActorLocation();
}

bool ULockOnSystemComponent::ShouldBreakLineOfSight() const
{
	if (!LockedOnTargetActor)
	{
		return true;
	}

	TArray<AActor*> ActorsToIgnore = GetAllActorsOfClass(TargetableActors);
	ActorsToIgnore.Remove(LockedOnTargetActor);

	FHitResult HitResult;
	const bool bHit = LineTrace(HitResult, LockedOnTargetActor, ActorsToIgnore);
	if (bHit && HitResult.GetActor() != LockedOnTargetActor)
	{
		return true;
	}

	return false;
}

void ULockOnSystemComponent::BreakLineOfSight()
{
	bIsBreakingLineOfSight = false;
	if (ShouldBreakLineOfSight())
	{
		TargetLockOff();
	}
}

void ULockOnSystemComponent::ControlRotation(const bool ShouldControlRotation) const
{
	if (!IsValid(OwnerPawn))
	{
		return;
	}

	OwnerPawn->bUseControllerRotationYaw = ShouldControlRotation;

	UCharacterMovementComponent* CharacterMovementComponent = OwnerPawn->FindComponentByClass<UCharacterMovementComponent>();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->bOrientRotationToMovement = !ShouldControlRotation;
	}
}

bool ULockOnSystemComponent::IsInViewport(const AActor* TargetActor) const
{
	if (!IsValid(OwnerPlayerController))
	{
		return true;
	}

	FVector2D ScreenLocation;
	OwnerPlayerController->ProjectWorldLocationToScreen(TargetActor->GetActorLocation(), ScreenLocation);

	FVector2D ViewportSize;
	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);

	return ScreenLocation.X > 0 && ScreenLocation.Y > 0 && ScreenLocation.X < ViewportSize.X && ScreenLocation.Y < ViewportSize.Y;
}
