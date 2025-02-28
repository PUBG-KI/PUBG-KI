// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/WeaponPartsSlot.h"

#include "Components/Image.h"

void UWeaponPartsSlot::NativeConstruct()
{
	Super::NativeConstruct();

	Image_Hovered->SetVisibility(ESlateVisibility::Collapsed);
	Image_Pressed->SetVisibility(ESlateVisibility::Collapsed);
	Image_Disabled->SetVisibility(ESlateVisibility::Collapsed);
	Image_Parts->SetVisibility(ESlateVisibility::Collapsed);
	Image_Equipable->SetVisibility(ESlateVisibility::Collapsed);
	//Image_Normal->SetVisibility(ESlateVisibility::Collapsed);
}
