// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/GA_Pickup.h"

#include "AbilitySystemComponent.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "Component/Movement/PlayerMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PlayerCharacter.h"
#include "Component/Inventory/InventoryComponent.h"
#include "AbilitySystem/AbilityTask/PlayMontageAndWaitForEvent.h"
#include "Item/ItemBase.h"

UGA_Pickup::UGA_Pickup()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Pickup::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData)
{
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	UInventoryComponent* InventoryComponent = PUBGPlayer->GetInventoryComponent();
	if (!InventoryComponent)
	{UE_LOG(LogTemp, Warning, TEXT("InventoryComponent is null!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
	AItemBase* Item = InventoryComponent->GetItem();
	if (!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item is null!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
	float ItemOfZ = Item->GetItemOfZ();
	if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Rifle"))))
	{
	}
	else if (UBaseFunctionLibrary::NativeActorHasTag(
		PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Melee"))))
	{
	}
	else if (UBaseFunctionLibrary::NativeActorHasTag(
		PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Grenade"))))
	{
	}
	else
	{
		UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(PUBGPlayer->GetMovementComponent());
		if (!MovementComponent->IsFalling())
		{
			if (MovementComponent->RequestToStartProne)
			{
				
			}
			else if (MovementComponent->bWantsToCrouch)
			{
				
			}
			else if (!MovementComponent->RequestToStartProne && !MovementComponent->bWantsToCrouch)
			{	
				if (ItemOfZ<500.f&&ItemOfZ>0.f) //하단 STAND모션
				{
					if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
					{
						if (UnarmedPickupMontage[(uint8)EMontageType::Low] != nullptr)
						{
							UE_LOG(LogTemp, Warning, TEXT("dd"));
							UAnimMontage* SelectedMontage = UnarmedPickupMontage[(uint8)EMontageType::Low];
							UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, SelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
							Task->OnBlendOut.AddDynamic(this, &UGA_Pickup::OnCompleted);
							Task->OnCompleted.AddDynamic(this, &UGA_Pickup::OnCompleted);
							
							Task->ReadyForActivation();
							
						}
					}
				}
			}
		}
	}
	EndAbility(Handle,ActorInfo, ActivationInfo, false, true);
}

void UGA_Pickup::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


