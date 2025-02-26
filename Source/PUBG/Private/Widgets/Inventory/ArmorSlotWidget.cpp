// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/ArmorSlotWidget.h"

#include "Components/Image.h"

void UArmorSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Image_Normal->SetVisibility(ESlateVisibility::Collapsed);
	Image_Hovered->SetVisibility(ESlateVisibility::Collapsed);
	Image_Pressed->SetVisibility(ESlateVisibility::Collapsed);
	Image_Disabled->SetVisibility(ESlateVisibility::Collapsed);
	Image_Armor->SetVisibility(ESlateVisibility::Collapsed);
	
}
