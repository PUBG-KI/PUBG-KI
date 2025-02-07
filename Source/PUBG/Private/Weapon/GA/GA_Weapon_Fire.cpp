// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GA/GA_Weapon_Fire.h"
#include "Camera/CameraComponent.h"
#include "Character/PlayerCharacter.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UGA_Weapon_Fire::SetDefault()
{
	if (GetPlayerCharacterFromActorInfo()->GetInventoryComponent()->GetCurrentWeapon_GunBase())
	{
		// current weapon Type-[Gun_Base]
		Gun_Base = GetPlayerCharacterFromActorInfo()->GetInventoryComponent()->GetCurrentWeapon_GunBase();

		// Weapon DT_Data
		WeaponData = Gun_Base->GetWeaponDataAsset();

		// bulletCount
		Gun_Base->SetBulletArom(Gun_Base->GetBulletArmo() - 1);
	}
}

FVector UGA_Weapon_Fire::GetVelocityVector()
{
	FVector LineTraceStart = GetPlayerCharacterFromActorInfo()->GetFollowCamera()->GetComponentLocation();
	FVector LineTraceEnd = LineTraceStart + (GetPlayerCharacterFromActorInfo()->GetFollowCamera()->GetForwardVector() * 10000.0);

	FHitResult HitResult;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetPlayerCharacterFromActorInfo()); // 자기 자신은 제외
	
	GetWorld()->LineTraceSingleByChannel(HitResult, LineTraceStart, LineTraceEnd, ECC_GameTraceChannel3, CollisionParams);

	if (HitResult.bBlockingHit)
	{
		FVector VelocityVector = UKismetMathLibrary::GetDirectionUnitVector(Gun_Base->GetWeaponMagSocketLocation(), HitResult.ImpactPoint);
		// UKismetSystemLibrary::DrawDebugLine(GetWorld(), LineTraceStart, HitResult.ImpactPoint, FLinearColor::Green, 100.0f, 1.0f);
		
		return VelocityVector;
	}
	else
	{
		// UKismetSystemLibrary::DrawDebugLine(GetWorld(), HitResult.ImpactPoint, LineTraceEnd, FLinearColor::Red, 100.0f, 1.0f);
		return GetPlayerCharacterFromActorInfo()->GetFollowCamera()->GetForwardVector();
	}
}

