// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GA/GA_Weapon_Reload.h"
#include "Character/PlayerCharacter.h"
#include "Component/Movement/PlayerMovementComponent.h"

UAnimMontage* UGA_Weapon_Reload::selectCurrentMontage_isProne_Reload(UAnimMontage* ReloadCharge_TrueMontage,
	UAnimMontage* ReloadCharge_falseMontage, UAnimMontage* ReloadTectical_TrueMontage, UAnimMontage* ReloadTectical_falseMontage,
	bool Selectbool)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(PlayerCharacter->GetMovementComponent());
	
	if (MovementComponent->RequestToStartProne)
	{
		if (Selectbool)
		{
			return ReloadCharge_TrueMontage;
		}
		return ReloadTectical_TrueMontage;
	}

	if (Selectbool)
	{
		return ReloadCharge_falseMontage;
	}
	return ReloadTectical_falseMontage;
}
