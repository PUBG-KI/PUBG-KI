// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/GA/GA_Prone.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseGameplayTag.h"
#include "Character/PlayerCharacter.h"
#include "BaseLibrary/BaseFunctionLibrary.h"

UGA_Prone::UGA_Prone()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
}

void UGA_Prone::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	APlayerCharacter* Player = GetPlayerCharacterFromActorInfo();
	
	if (Player)
	{
		if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(BaseGameplayTag::Player_State_Prone)) //Prone -> Stand
		{
			Player->bIsProne = false;
			UBaseFunctionLibrary::RemoveGameplayTagFromActor(Player, BaseGameplayTag::Player_State_Prone);
			EndAbility(Handle, ActorInfo, ActivationInfo,true, false);
		}
		else // another -> Prone
		{
			BP_PlayMontageAndWait();
		}
	}
}

