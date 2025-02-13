// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Guns/Gun_Base.h"

#include "BaseLibrary/DataEnum/ItemEnum.h"

AGun_Base::AGun_Base()
{
	ScopeMesh = CreateDefaultSubobject<UStaticMeshComponent>("ScopeMesh");
	ScopeMesh->SetupAttachment(WeaponSkeletalMeshComponent,FName("ScopeSocket"));

	MagMesh = CreateDefaultSubobject<UStaticMeshComponent>("MagMesh");
	MagMesh->SetupAttachment(WeaponSkeletalMeshComponent, FName("MagSocket"));

	MuzzleMesh = CreateDefaultSubobject<UStaticMeshComponent>("MuzzleMesh");
	MuzzleMesh->SetupAttachment(WeaponSkeletalMeshComponent,FName("MuzzleSocket"));

	GripMesh = CreateDefaultSubobject<UStaticMeshComponent>("GripMesh");
	GripMesh->SetupAttachment(WeaponSkeletalMeshComponent, FName("GripSocket"));

	LoopsMesh = CreateDefaultSubobject<UStaticMeshComponent>("LoopsMesh");
	LoopsMesh->SetupAttachment(WeaponSkeletalMeshComponent, FName("LoopsSocket"));

	//SetReplicates(true);
	bReplicates = true;

	// 이준수
	EquipSlot = EEquippedItemCategory::PrimarySlot;
}

void AGun_Base::SetFiremode(EFiremodes Firemodes)
{
	this->FireMode = Firemodes;
}

void AGun_Base::ChangeFiremode_AR(EFiremodes _Firemode)
{
	if (IgnoreMode == _Firemode)
	{
		this->FireMode = EFiremodes::Semi_Auto;
	}
	else
	{
		this->FireMode = _Firemode;
	}
}

void AGun_Base::SetMagToHandSocekt()
{
	//if (MagMesh->GetAttachParent())
}

void AGun_Base::SetBulletArom(float Armo)
{
	this->BulletArmo = Armo;
}

FVector AGun_Base::GetWeaponMagSocketLocation() const
{
	return WeaponSkeletalMeshComponent->GetSocketLocation(FName("ProjectileSocket"));
}

void AGun_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGun_Base, BulletArmo);
	DOREPLIFETIME(AGun_Base, MagMesh);
}

void AGun_Base::Server_SetBulletArom_Implementation(float Armo)
{
	SetBulletArom(Armo);
}

bool AGun_Base::Server_SetBulletArom_Validate(float Armo)
{
	return true;
}


