// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/GA/GA_Sprint.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	APlayerCharacter* Player = GetPlayerCharacterFromActorInfo();
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Player is not exists"));
		return;
	}
	if (Player)
	{
		UCharacterMovementComponent* MovementComp = Player->GetCharacterMovement();
		if (MovementComp)
		{
			// 현재 이동 방향이 뒤로가고 있는지 체크
			FVector Velocity = MovementComp->Velocity;
			bool bIsMovingBackward = FVector::DotProduct(Velocity, Player->GetActorForwardVector()) < 0.f;

			// 뒤로 이동 중이 아니면 스프린트 속도를 600으로 설정
			if (!bIsMovingBackward)
			{
				MovementComp->MaxWalkSpeed = 600.f;
			}
		}
	}
	
}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	APlayerCharacter* Player = GetPlayerCharacterFromActorInfo();
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Player is not exists"));
		return;
	}
	if (Player)
	{
		UCharacterMovementComponent* MovementComp = Player->GetCharacterMovement();
		if (MovementComp)
		{
			MovementComp->MaxWalkSpeed = 400.f;
		}
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

