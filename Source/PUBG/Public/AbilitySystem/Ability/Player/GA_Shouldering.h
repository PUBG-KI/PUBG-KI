// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "GA_Shouldering.generated.h"

class UPUBGSpringArmComponent;
/**
 * 
 */
UCLASS()
class PUBG_API UGA_Shouldering : public UPlayerGameplayAbility
{
	GENERATED_BODY()
	UGA_Shouldering();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData);
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	

private:
	FTimerHandle CameraBoomTimerHandle;
	UPROPERTY()
	UPUBGSpringArmComponent* PlayerCameraBoom;
	bool bIsOnce;
	FVector InitialOffset;
	FVector CurrentOffset;
	FVector AddOffset;
	FVector SubOffset;
};
