// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_Base.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeapon_Base::AWeapon_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponSkeletalMeshComponent;

	//SetReplicates(true);
	bReplicates = true;

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

// inline void AWeapon_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//  {
//  	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//  
//  	DOREPLIFETIME(AWeapon_Base, PlayerWeaponData);
//  	DOREPLIFETIME(AWeapon_Base, GrantedAbilitySpecHandles);
//  	
//  }

