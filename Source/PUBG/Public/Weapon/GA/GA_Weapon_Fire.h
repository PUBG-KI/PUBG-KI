// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "Weapon/DataTable/DT_Weapon.h"
#include "Weapon/Guns/Gun_Base.h"
#include "GA_Weapon_Fire.generated.h"


class AWeapon_Base;
class AGun_Base;

/**
 * 
 */
UCLASS()
class PUBG_API UGA_Weapon_Fire : public UPlayerGameplayAbility
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FWeaponData WeaponData;

	UPROPERTY(EditAnywhere)
	AWeapon_Base* Weapon_Base;
	
	UPROPERTY(EditAnywhere)
	AGun_Base* Gun_Base;

public:
	UFUNCTION(BlueprintCallable, Category = "GA_weapon")
	void SetDefault();

	UFUNCTION(BlueprintCallable, Category = "GA_weapon")
	FVector GetVelocityVector();

	UFUNCTION(BlueprintCallable, Category = "GA_weapon")
	FWeaponData GetWeaponData() const {return WeaponData;}

	UFUNCTION(BlueprintPure, Category = "GA_weapon")
	FVector GetProjectileSocketLocation() const {return Gun_Base->GetWeaponMagSocketLocation();}
	
};
