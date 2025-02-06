// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "GA_Pickup.generated.h"

/**
 * 
 */

UENUM()
enum class EMontageType : uint8
{
	High UMETA(DisplayName = "High"),
	Middle UMETA(DisplayName = "Middle"),
	Low UMETA(DisplayName = "Low")
};

UCLASS()
class PUBG_API UGA_Pickup : public UPlayerGameplayAbility
{
	GENERATED_BODY()
	UGA_Pickup();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PickupMontage")
	TArray<UAnimMontage*> UnarmedPickupMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PickupMontage")
	TArray< UAnimMontage*> RiflePickupMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PickupMontage")
	TArray<UAnimMontage*> GrenadePickupMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PickupMontage")
	TArray<UAnimMontage*> MeleePickupMontage;

public:
	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	
	
};
