// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/GA_ProneMotion.h"
#include "Character/PlayerCharacter.h"
#include "Component/PUBGSpringArmComponent.h"
#include "Component/Movement/PlayerMovementComponent.h"


UGA_ProneMotion::UGA_ProneMotion()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ProneMotion::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	UPlayerMovementComponent* MovementComponent = Cast<
		UPlayerMovementComponent>(PUBGPlayer->GetMovementComponent());
	if (MovementComponent->IsFalling() && PUBGPlayer->bAnimationIsPlaying)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (MovementComponent->RequestToStartProne) //누워있는 상태면
	{
		if (!PUBGPlayer->bAnimationIsPlaying)
		{
			FVector UnProneCameraOffset = FVector(0, 0, 80.f);

			PUBGPlayer->GetCameraBoom()->TimelineAddOffset(UnProneCameraOffset, 0.2f);
			MovementComponent->StopProne();
		}
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (MovementComponent->IsCrouching())
	{
		if (!PUBGPlayer->bAnimationIsPlaying)
		{
			PUBGPlayer->UnCrouch();
			MovementComponent->StartProne();
			FVector ProneCameraOffset = FVector(0, 0, -40.f);
			PUBGPlayer->GetCameraBoom()->TimelineAddOffset(ProneCameraOffset, 0.2f);
		}
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	if (!PUBGPlayer->bAnimationIsPlaying)
	{
		FVector ProneCameraOffset = FVector(0, 0, -80.f);
		PUBGPlayer->GetCameraBoom()->TimelineAddOffset(ProneCameraOffset, 0.2f); //카메라 오프셋 이동
		MovementComponent->StartProne();
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UGA_ProneMotion::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
