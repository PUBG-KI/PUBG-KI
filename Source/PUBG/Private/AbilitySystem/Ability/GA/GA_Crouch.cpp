// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/GA/GA_Crouch.h"

#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"



void UGA_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	APlayerCharacter* Player = GetPlayerCharacterFromActorInfo();
	if (Player)
	{
		if (Player->bIsCrouched)//(Player->GetPlayerStance() == EPlayerStance::Crouch)
		{
			Player->UnCrouch();
			Player->bIsCrouched = false;
			//Player->SetPlayerStance(EPlayerStance::Stand);
			//Player->GetCharacterMovement()->MaxWalkSpeedCrouched +=100.0f;
			EndAbility(Handle, ActorInfo, ActivationInfo,true, false);
		}
		else
		{
			Player->Crouch();
			Player->bIsCrouched = true;
			//Player->SetPlayerStance(EPlayerStance::Crouch);
			//Player->GetCharacterMovement()->MaxWalkSpeedCrouched -=100.0f;
		}
	}
}

void UGA_Crouch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
