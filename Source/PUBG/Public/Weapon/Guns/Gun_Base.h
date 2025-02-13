// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon_Base.h"
#include "Weapon/DataTable/DT_Weapon.h"
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
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts", replicated)
	UStaticMeshComponent* MagMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts")
	UStaticMeshComponent* GripMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UStaticMeshComponent* LoopsMesh;

	UPROPERTY(EditDefaultsOnly, Category = "FireMode")
	EFiremodes IgnoreMode;

	UPROPERTY(EditDefaultsOnly, Category = "FireMode")
	EFiremodes FireMode = EFiremodes::Single;

	UPROPERTY(EditDefaultsOnly, Replicated)
	float BulletArmo;

	UPROPERTY(EditDefaultsOnly, Replicated)
	FVector WeaponProjectileSocketLocation;

public:
	UFUNCTION(BlueprintCallable, Category = "FireMode")
	FORCEINLINE EFiremodes GetFireMode() const { return FireMode; }

	UFUNCTION(BlueprintCallable, category = "FireMode")
	void SetFiremode(EFiremodes Firemodes);

	UFUNCTION(BlueprintCallable, category = "FireMode")
	void ChangeFiremode_AR(EFiremodes _Firemode);

	UFUNCTION(BlueprintCallable, Category = "Mag")
	UStaticMeshComponent* GetMagToHandSocekt() const {return MagMesh;};
	
	UFUNCTION(BlueprintCallable, Category = "Mag")
	void SetMagToHandSocekt();

	UFUNCTION(BlueprintCallable, Category = "DataAsset")
	float GetBulletArmo() const {return BulletArmo;}

	UFUNCTION(BlueprintCallable, Category = "DataAsset")
	void SetBulletArom(float Armo);

	UFUNCTION(BlueprintCallable, Category = "Mag")
	FVector GetWeaponMagSocketLocation() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Mag")
	void Server_SetBulletArom(float Armo);

	// 이준수
	// 이준수
	EEquippedItemCategory GetEquipSlot() const override { return EquipSlot; }
	
};


