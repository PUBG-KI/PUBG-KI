// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "GA_Prone.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API UGA_Prone : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Prone();
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* StandToProne;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* ProneToStand;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION(BlueprintImplementableEvent)
	void BP_PlayMontageAndWait();
	
};
