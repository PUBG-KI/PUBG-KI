// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "Containers/Deque.h"
#include "Weapon/Weapon_Base.h"
#include "GA_Number_Equip_UnEquip.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API UGA_Number_Equip_UnEquip : public UPlayerGameplayAbility
{
	GENERATED_BODY()



public:
	UFUNCTION(BlueprintPure)
	bool EqualCurrentWeapon(AWeapon_Base* _Weapon);

	UFUNCTION(BlueprintPure)
	bool IsValidCurrentWeapon();

	UFUNCTION(BlueprintCallable)
	void Attach_HandToBack_Weapon(FName _SocketName);

	UFUNCTION(BlueprintCallable)
	void Attach_BackToHand_Weapon(FName _SocketName, AWeapon_Base* _Weapon);

	UFUNCTION(BlueprintPure)
	FName GetSocketName() const {return SocketName;}

	UFUNCTION(BlueprintCallable)
	void setSocketName(FName NewName);

	UFUNCTION(Blueprintpure)
	bool EqualCurrentWeaponToPrimaryWeaponSlot();

	UFUNCTION(Blueprintpure)
	bool EqualLastCurrentWeaponToPrimaryWeaponSlot();

private:
	UPROPERTY()
	FName SocketName;
};
