// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "GA_Weapon_Reload.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API UGA_Weapon_Reload : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	UAnimMontage* selectCurrentMontage_isProne_Reload(UAnimMontage* ReloadCharge_TrueMontage,UAnimMontage* ReloadCharge_falseMontage, UAnimMontage* ReloadTectical_TrueMontage, UAnimMontage* ReloadTectical_falseMontage,bool Selectbool);
	
};
