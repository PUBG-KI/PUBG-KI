// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GA/GA_Number_Equip_UnEquip.h"

#include "Character/PlayerCharacter.h"
#include "Component/Inventory/InventoryComponent.h"

bool UGA_Number_Equip_UnEquip::EqualCurrentWeapon(AWeapon_Base* _Weapon)
{
	if (_Weapon)
	{
		APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
		if (PlayerCharacter)
		{
			if (PlayerCharacter->GetInventoryComponent()->GetCurrentWeapon() == _Weapon)
			{
				return true;
			}
		}
	}
	return false;
}

bool UGA_Number_Equip_UnEquip::IsValidCurrentWeapon()
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	if (PlayerCharacter)
	{
		if (PlayerCharacter->GetInventoryComponent()->GetCurrentWeapon() != nullptr)
		{
			return true;
		}
	}
	return false;
}

void UGA_Number_Equip_UnEquip::Attach_HandToBack_Weapon(FName _SocketName)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	if (PlayerCharacter)
	{
		if (AWeapon_Base* Weapon_Base = PlayerCharacter->GetInventoryComponent()->GetCurrentWeapon())
		{
			//PlayerCharacter->GetInventoryComponent()->SetLastCurrentWeapon(PlayerCharacter->GetInventoryComponent()->GetCurrentWeapon());
			
			FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			Weapon_Base->AttachToComponent(PlayerCharacter->GetMesh(), Rules, _SocketName);

			PlayerCharacter->GetInventoryComponent()->SetCurrentWeapon(nullptr);
			
		}
	}
}

void UGA_Number_Equip_UnEquip::Attach_BackToHand_Weapon(FName _SocketName, AWeapon_Base* _Weapon)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	if (PlayerCharacter)
	{
		if (_Weapon)
		{
			FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			_Weapon->AttachToComponent(PlayerCharacter->GetMesh(), Rules, _SocketName);

			PlayerCharacter->GetInventoryComponent()->SetCurrentWeapon(_Weapon);
			PlayerCharacter->GetInventoryComponent()->SetLastCurrentWeapon(PlayerCharacter->GetInventoryComponent()->GetCurrentWeapon());
			
		}
	}
}

void UGA_Number_Equip_UnEquip::setSocketName(FName NewName)
{
	this->SocketName = NewName;
}

bool UGA_Number_Equip_UnEquip::EqualCurrentWeaponToPrimaryWeaponSlot()
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	if (PlayerCharacter)
	{
		UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
		
		if (InventoryComponent && (InventoryComponent->GetCurrentWeapon() == InventoryComponent->GetPrimarySlotWeapon()))
		{
			SocketName = FName("slot_primarySocket");
			return true;
		}
	}
	SocketName = FName("slot_secondarySocket");
	return false;
}

bool UGA_Number_Equip_UnEquip::EqualLastCurrentWeaponToPrimaryWeaponSlot()
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	if (PlayerCharacter)
	{
		UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
		
		if (InventoryComponent && (InventoryComponent->GetLastCurrentWeapon() == InventoryComponent->GetPrimarySlotWeapon()))
		{
			SocketName = FName("slot_primarySocket");
			return true;
		}
	}
	SocketName = FName("slot_secondarySocket");
	return false;
}

