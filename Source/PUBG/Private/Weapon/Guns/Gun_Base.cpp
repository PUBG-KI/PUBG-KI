// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Guns/Gun_Base.h"

AGun_Base::AGun_Base()
{
	ScopeMesh = CreateDefaultSubobject<UStaticMeshComponent>("ScopeMesh");
	ScopeMesh->AttachToComponent(WeaponSkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("ScopeSocket"));

	MagMesh = CreateDefaultSubobject<UStaticMeshComponent>("MagMesh");
	MagMesh->AttachToComponent(WeaponSkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("MagSocket"));

	MuzzleMesh = CreateDefaultSubobject<UStaticMeshComponent>("MuzzleMesh");
	MuzzleMesh->AttachToComponent(WeaponSkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("MuzzleSocket"));

	GripMesh = CreateDefaultSubobject<UStaticMeshComponent>("GripMesh");
	GripMesh->AttachToComponent(WeaponSkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("GripSocket"));

	LoopsMesh = CreateDefaultSubobject<UStaticMeshComponent>("LoopsMesh");
	LoopsMesh->AttachToComponent(WeaponSkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LoopsSocket"));
	
}

void AGun_Base::SetFiremode(EFiremodes Firemodes)
{
	this->FireMode = Firemodes;
}

void AGun_Base::SetMagToHandSocekt()
{
	//if (MagMesh->GetAttachParent())
}

void AGun_Base::SetBulletArom(float Armo)
{
	this->BulletArmo = Armo;
}
