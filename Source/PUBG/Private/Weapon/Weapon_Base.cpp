// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_Base.h"

#include "BaseLibrary/DataEnum/ItemEnum.h"
#include "Components/SceneCaptureComponent.h"
#include "Engine/TextureRenderTarget.h"
#include "Net/UnrealNetwork.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"


// Sets default values
AWeapon_Base::AWeapon_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponSkeletalMeshComponent;

	//SetReplicates(true);
	bReplicates = true;

	// 이준수
	EquipSlot = EEquippedItemCategory::PrimarySlot;

	SceneCapture->SetupAttachment(RootComponent);
	SceneCapture->SetWorldLocationAndRotation(FVector(20.0f, -60.0f, 0.0f), FRotator(0.0f, 90.0f, 0.0f));
	
	
}

// Called when the game starts or when spawned
void AWeapon_Base::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon_Base::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles)
{
	GrantedAbilitySpecHandles = SpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AWeapon_Base::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}

void AWeapon_Base::SetWeaponDataAsset(FWeaponData WeaponData)
{
	this->WeaponDataAsset = WeaponData;
}


// UTextureRenderTarget2D* AWeapon_Base::CreateRenderTarget2D(int32 width, int32 height, bool makeHDR)
// {
// 	
// }
//
// bool AWeapon_Base::SaveRenderTarget(UTextureRenderTarget2D* renderTarget, FString path, FString fileName)
// {
// }

// inline void AWeapon_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//  {
//  	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//  
//  	DOREPLIFETIME(AWeapon_Base, PlayerWeaponData);
//  	DOREPLIFETIME(AWeapon_Base, GrantedAbilitySpecHandles);
//  	
//  }

