// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/ArmorSlotWidget.h"

#include "Component/Equipped/EquippedComponent.h"
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

FReply UArmorSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	UE_LOG(LogTemp, Warning, TEXT("UArmorSlotWidget::NativeOnPreviewMouseButtonDown"));

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (EquippedComponent)
		{
			EquippedComponent->ServerDropArmor(ArmorIndex);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UArmorSlotWidget::NativeOnPreviewMouseButtonDown = EquippedComponent None"));
		}
	}
	return FReply::Handled();
}

void UArmorSlotWidget::InitArmorSlot()
{
	Image_Normal->SetVisibility(ESlateVisibility::Visible);
	Image_Hovered->SetVisibility(ESlateVisibility::Collapsed);
	Image_Pressed->SetVisibility(ESlateVisibility::Collapsed);
	Image_Disabled->SetVisibility(ESlateVisibility::Collapsed);
	Image_Armor->SetVisibility(ESlateVisibility::Collapsed);
}
