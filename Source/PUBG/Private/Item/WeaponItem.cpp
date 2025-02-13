// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/WeaponItem.h"

#include "Component/EquippedComponent.h"
#include "Component/Inventory/InventoryComponent.h"


AWeaponItem::AWeaponItem()
{
	bReplicates = true;
	SetReplicates(true);
}

void AWeaponItem::InteractWith_Implementation(APlayerCharacter* Character)
{
	//Super::InteractWith_Implementation(Character);

	if (!HasAuthority()) // 클라
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponItem : InteractWith_Implementation!!!!!!!!!!!!!"));

		UInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
		InventoryComponent->SetItem(this); // 리플리케이트가 느림
		InventoryComponent->ServerSetItem(this); // 

		UEquippedComponent* EquippedComponent = Character->GetEquippedComponent();
		EquippedComponent->ServerEquipMainItem(InventoryComponent->GetItem());
		
	}

	
}
