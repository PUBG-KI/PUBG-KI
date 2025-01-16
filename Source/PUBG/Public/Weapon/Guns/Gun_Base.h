// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon_Base.h"
#include "Gun_Base.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EFiremodes : uint8
{
	Auto = 0,
	Single,
	Semi_Auto,
};

UCLASS()
class PUBG_API AGun_Base : public AWeapon_Base
{
	GENERATED_BODY()

public:
	AGun_Base();
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts")
	UStaticMeshComponent* ScopeMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts")
	UStaticMeshComponent* MuzzleMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts")
	UStaticMeshComponent* MagMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts")
	UStaticMeshComponent* GripMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UStaticMeshComponent* LoopsMesh;

	UPROPERTY(EditDefaultsOnly, Category = "FireMode")
	EFiremodes FireMode = EFiremodes::Auto;
	

public:
	UFUNCTION(BlueprintCallable, Category = "FireMode")
	FORCEINLINE EFiremodes GetFireMode() const { return FireMode; }

	UFUNCTION(BlueprintCallable, category = "FireMode")
	void SetFiremode(EFiremodes Firemodes);

	
};
