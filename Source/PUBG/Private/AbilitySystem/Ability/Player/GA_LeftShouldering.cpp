// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/GA_LeftShouldering.h"
#include "Component/PUBGSpringArmComponent.h"
#include "Character/PlayerCharacter.h"

UGA_LeftShouldering::UGA_LeftShouldering()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_LeftShouldering::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("Activating Ability"));
	PlayerCameraBoom = GetPlayerCharacterFromActorInfo()->GetCameraBoom();
	if (GetPlayerCharacterFromActorInfo()->GetCameraMode() == PlayerCameraMode::FPPCamera)
	{
		//PlayerCameraBoom->SetWanstReversePlaying(false);
		FVector OffsetDelta = FVector(110.f, -10.f, 0.f);
		float Duration = 0.2f;
		PlayerCameraBoom->TimelineAddOffset(OffsetDelta, Duration);
	}
}

void UGA_LeftShouldering::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (GetPlayerCharacterFromActorInfo()->GetCameraMode() == PlayerCameraMode::FPPCamera)
	{
		//PlayerCameraBoom->SetWanstReversePlaying(true);
		FVector OffsetDelta = FVector(0.f, 0.f, 0.f);
		float Duration = 0.2f;
		PlayerCameraBoom->TimelineAddOffset(OffsetDelta, Duration);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
