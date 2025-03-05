// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "Weapon/Guns/Gun_Base.h"
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

	UFUNCTION(BlueprintCallable)
	void CheckZoom();

	UFUNCTION(BlueprintPure)
	float EquippedMagPart_BulletCalulate(AGun_Base* CurrentWeapon, float CurrentBullet, float IsMag_changed_BulletValue);

	UFUNCTION(BlueprintCallable)
	void SetReloadBullet_CalulateBullet(AGun_Base* CurrentWeapon, float CurrentBullet, float IsMag_changed_BulletValue, FName BulletName);

	UFUNCTION(blueprintPure)
	bool CheckCurrentBullet(AGun_Base* CurrentWeapon, float DT_BulletArmo, FName BulletName);

	UFUNCTION()
	FName CheckBulletTypeName(FName BulletName);
};
