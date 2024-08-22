// Copyright Epic Games, Inc. All Rights Reserved.

#include "PBMeleeWeaponInstance.h"

#include "PBRangedWeaponInstance.h"
#include "NativeGameplayTags.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/LyraCameraComponent.h"
#include "Physics/PhysicalMaterialWithTags.h"
#include "PBWeaponInstance.h"

UPBMeleeWeaponInstance::UPBMeleeWeaponInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPBMeleeWeaponInstance::PostLoad()
{
	Super::PostLoad();

#if WITH_EDITOR
	UpdateDebugVisualization();
#endif
}

#if WITH_EDITOR
void UPBMeleeWeaponInstance::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UpdateDebugVisualization();
}

void UPBMeleeWeaponInstance::UpdateDebugVisualization()
{
	
}
#endif

void UPBMeleeWeaponInstance::OnEquipped()
{
	Super::OnEquipped();
}

void UPBMeleeWeaponInstance::OnUnequipped()
{
	Super::OnUnequipped();
}

void UPBMeleeWeaponInstance::Tick(float DeltaSeconds)
{
	/*APawn* Pawn = GetPawn();
	check(Pawn != nullptr);*/

#if WITH_EDITOR
	UpdateDebugVisualization();
#endif
}

float UPBMeleeWeaponInstance::GetDistanceAttenuation(float Distance, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags) const
{
	return 1.0f;
}

float UPBMeleeWeaponInstance::GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysicalMaterial, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags) const
{
	float CombinedMultiplier = 1.0f;
	if (const UPhysicalMaterialWithTags* PhysMatWithTags = Cast<const UPhysicalMaterialWithTags>(PhysicalMaterial))
	{
		for (const FGameplayTag MaterialTag : PhysMatWithTags->Tags)
		{
			if (const float* pTagMultiplier = MaterialDamageMultiplier.Find(MaterialTag))
			{
				CombinedMultiplier *= *pTagMultiplier;
			}
		}
	}

	return CombinedMultiplier;
}
