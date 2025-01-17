// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryWidget.h"

#include "Components/WrapBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Inventory/ItemSlotWidget.h"


UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InventoryComponent = nullptr;
	ItemSlotWidget  = nullptr;
	WrapBox_Inventory  = nullptr;
}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	
}

void UInventoryWidget::UpdateInventoryWidget()
{
	GetWrapBox_Inventory()->ClearChildren();
	
	TArray<FItemSlotStruct> ItemSlot = InventoryComponent->GetContent();
	for (int i = 0; i < ItemSlot.Num(); i++)
	{
		UClass* ItemSlotWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_ItemSlot.WBP_ItemSlot_C"));
		if (ItemSlotWidgetClass)
		{
			ItemSlotWidget = CreateWidget<UItemSlotWidget>(GetWorld(), ItemSlotWidgetClass);
			if (ItemSlotWidget != nullptr)
			{
				ItemSlotWidget->SetItemName(ItemSlot[i].ItemName);
				ItemSlotWidget->SetQuantity(ItemSlot[i].Quantity);
				ItemSlotWidget->SetInventoryComponent(InventoryComponent);
				ItemSlotWidget->SetIndex(i);
				ItemSlotWidget->UpdateItemSlotWidget();
				GetWrapBox_Inventory()->AddChildToWrapBox(ItemSlotWidget);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemSlot Widget not found"));
		}
	}
}

void UInventoryWidget::AddNewChild()
{
	GetWrapBox_Inventory()->AddChildToWrapBox(ItemSlotWidget);
}
