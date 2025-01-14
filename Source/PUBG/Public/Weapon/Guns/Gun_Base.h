// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon_Base.h"
#include "Gun_Base.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API AGun_Base : public AWeapon_Base
{
	GENERATED_BODY()

public:
	AGun_Base();
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts")
	FName ScopeSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts")
	FName muzzleSocket;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts")
	FName MagSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts")
	FName ChokeLoops;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	FName ShootProjectile;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts")
	USkeletalMeshComponent* AttachablePart;


public:
	virtual void AttachPartsToSockets();
	
};
