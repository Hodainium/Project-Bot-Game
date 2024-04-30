// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Weapons/PBWeaponComponent.h"

#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UPBWeaponComponent::UPBWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPBWeaponComponent::ActuallyUpdateDamageInstigatedTime()
{
	// If our LastWeaponDamageInstigatedTime was not very recent, clear our LastWeaponDamageScreenLocations array
	UWorld* World = GetWorld();
	if (World->GetTimeSeconds() - TimeOfLastAttack > 0.1)
	{
		//LastWeaponDamageScreenLocations.Reset();
	}
	TimeOfLastAttack = World->GetTimeSeconds();
}

void UPBWeaponComponent::RegisterWithAbilitySystem(UAbilitySystemComponent* ASC)
{

}

void UPBWeaponComponent::UnregisterWithAbilitySystem()
{
}

bool UPBWeaponComponent::CanAttack() const
{
	if(AttackCooldownTime <= GetWorld()->TimeSeconds - TimeOfLastAttack)
	{
		return true;
	}

	return false;
}

void UPBWeaponComponent::AddUnconfirmedServerSidEPBitMarkers(const FGameplayAbilityTargetDataHandle& InTargetData,
                                                           const TArray<FHitResult>& FoundHits)
{
	UnconfirmedMeleeHitMarkers.Emplace(InTargetData.UniqueId);
}

void UPBWeaponComponent::HandleOnMeleEPBitEvent(AActor* HitActor, UPrimitiveComponent* HitComponent,
	const FVector& ImpactPoint, const FVector& ImpactNormal, const FGameplayTag& WeaponPartTag, FName HitBoneName,
	const FHitResult& HitResult)
{
	if(OnMeleeHitTaggedEvent.IsBound())
	{
		OnMeleeHitTaggedEvent.Broadcast(HitActor, HitComponent, ImpactPoint, ImpactNormal, WeaponPartTag, HitBoneName, HitResult);
	}
	//Do more here?

	if (AActor* HitActorLocal = HitResult.GetActor()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("HitActor named: %s"), *HitActorLocal->GetName());

		//AHCharacterBase* CharBase = Cast<AHCharacterBase>(HitActorLocal);

		//if (CharBase)
		//{
		//	UAbilitySystemComponent* TargetASC = CharBase->GetAbilitySystemComponent();

		//	if (TargetASC)
		//	{
		//		/*ensure(DamageGameplayEffect);

		//		FGameplayEffectContextHandle DamageContextHandle = AbilitySystemComponentRef->MakeEffectContext();
		//		DamageContextHandle.AddHitResult(HitResult);

		//		FGameplayEffectSpecHandle DamagEPBandle = AbilitySystemComponentRef->MakeOutgoingSpec(DamageGameplayEffect, AttributeSetBaseRef->GetLevel(), DamageContextHandle);
		//		DamagEPBandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), GunDamage);

		//		if (DamagEPBandle.IsValid())
		//		{
		//			FActiveGameplayEffectHandle AppliedEffect = AbilitySystemComponentRef->ApplyGameplayEffectSpecToTarget(*DamagEPBandle.Data.Get(), TargetASC);
		//		}*/
		//	}
		//}
	}
}

void UPBWeaponComponent::StartMeleeTrace()
{
	OnMeleeTraceStart.Broadcast();
}

void UPBWeaponComponent::StopMeleeTrace()
{
	OnMeleeTraceStop.Broadcast();
}

void UPBWeaponComponent::ClientConfirmTargetData_Implementation(uint16 UniqueId, bool bSuccess, const TArray<uint8>& HitReplaces)
{
	for (int i = 0; i < UnconfirmedMeleeHitMarkers.Num(); i++)
	{
		if (UnconfirmedMeleeHitMarkers[i] == UniqueId)
		{
			if (bSuccess)
			{
				UWorld* World = GetWorld();
				bool bFoundShowAsSuccessHit = false;
				
				// Only need to do this once
				if (!bFoundShowAsSuccessHit)
				{
					ActuallyUpdateDamageInstigatedTime();
				}

				bFoundShowAsSuccessHit = true;

				//LastWeaponDamageScreenLocations.Add(Entry);
				PlayHitMarkerDelegate.Broadcast();
			}

			UnconfirmedMeleeHitMarkers.RemoveAt(i);
			break;
		}
	}
}




