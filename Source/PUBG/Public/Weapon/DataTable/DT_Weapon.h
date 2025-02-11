// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DT_Weapon.generated.h"

UENUM(BlueprintType)
enum class EGunType : uint8
{
	AR,
	SR,
	DMR,
	SMG,
	SG,
	Pistol,
	Melee,
	Throwable,
};

UENUM(BlueprintType)
enum class EBulletType : uint8
{
	B_7_62 UMETA(DisplayName = "7.62mm"),
	B_5_56 UMETA(DisplayName = "5.56mm"),
	B_9 UMETA(DisplayName = "9mm"),
	B_12G UMETA(DisplayName = "12G"),
};


/**
 * 
 */

USTRUCT(Atomic, BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGunType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GunName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBulletType BulletType;

	// 기본 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Gun_Damage;

	// 기본 장전 탄창
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float Armo;

	// 총알 속도
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float Bullet_Speed;

	// 총알 단면적
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float Bullet_crossSection;

	// 총알 무게
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float bullet_Mass;

	// 총알 항력
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float Bullet_DragCoefficiency;

	// 반동 값 =====================================
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float Min_XRecoil;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float Max_XRecoil;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float Min_YRecoil;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float Max_YRecoil;

	// ===========================================
	

	
	

	

	
	
};

UCLASS()
class PUBG_API UDT_Weapon : public UDataTable
{
	GENERATED_BODY()
	
};
