// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryWidget.h"

#include "Component/NearArea/NearComponent.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Item/ItemBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Inventory/ItemSlotWidget.h"


UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InventoryComponent = nullptr;
	ItemSlotWidget  = nullptr;
	WrapBox_Inventory  = nullptr;

	ItemSlotWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_ItemSlot.WBP_ItemSlot_C"));
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
				FString text = TEXT("사용");
				NearItemSlotWidget->GetTextBlock_Use()->SetText(FText::FromString(text));
				GetWrapBox_Inventory()->AddChildToWrapBox(ItemSlotWidget);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemSlot Widget not found"));
		}
	}
}

void UInventoryWidget::UpdateNearItemSlotWidget()
{
	GetWrapBox_Near()->ClearChildren();
	TArray<AItemBase*> ItemSlot = NearComponent->GetGroundItem();
	UE_LOG(LogTemp, Warning, TEXT("GroundItem[0] : %s"), *NearComponent->GetGroundItem()[0]->GetItemStruct().Name.ToString());
	UE_LOG(LogTemp, Warning, TEXT("ItemSlot : %d"), ItemSlot.Num());
	for (int i = 0; i < ItemSlot.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ItemSlot Widget not found"));
		if (ItemSlotWidgetClass)
		{
			NearItemSlotWidget = CreateWidget<UItemSlotWidget>(GetWorld(), ItemSlotWidgetClass);
			if (NearItemSlotWidget != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Name : %s"), *ItemSlot[i]->GetItemStruct().Name.ToString());
				NearItemSlotWidget->SetItemName(ItemSlot[i]->GetItemStruct().Name);
				NearItemSlotWidget->SetQuantity(ItemSlot[i]->GetItemStruct().Quantity);
				NearItemSlotWidget->SetNearComponent(NearComponent);
				NearItemSlotWidget->SetInventoryComponent(InventoryComponent);
				NearItemSlotWidget->SetIndex(i);
				FString text = TEXT("줍기");
				NearItemSlotWidget->GetTextBlock_Use()->SetText(FText::FromString(text));
				//NearItemSlotWidget->GetTextBlock_Use()->SetText(LOCTEXT("UseText", "사용"));
				NearItemSlotWidget->UpdateItemSlotWidget();
				GetWrapBox_Near()->AddChildToWrapBox(NearItemSlotWidget);
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
