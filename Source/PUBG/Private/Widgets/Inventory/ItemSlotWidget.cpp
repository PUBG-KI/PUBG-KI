// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/ItemSlotWidget.h"

#include "BaseLibrary/DataStruct/ItemStruct.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DragDrop/DDInventorySlot.h"
#include "Widgets/Inventory/DragItemWidget.h"

void UItemSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void UItemSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
}

void UItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_Use->SetVisibility(ESlateVisibility::Collapsed);

	Button_ItemSlot->OnHovered.AddDynamic(this, &UItemSlotWidget::OnButton_ItemSlotHovered);
	Button_ItemSlot->OnUnhovered.AddDynamic(this, &UItemSlotWidget::OnButton_ItemSlotUnHovered);

}

void UItemSlotWidget::UpdateItemSlotWidget()
{
	FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");
	UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));

	if (DataTable)
	{
		if (ItemName != NAME_None)
		{
			FItemStruct* Row = DataTable->FindRow<FItemStruct>(ItemName, TEXT("Get Item Row"));

			Image_Icon->SetBrushFromTexture(Row->Image);
			Text_Quantity->SetText(FText::AsNumber(Quantity));
			Text_ItemName->SetText(FText::FromName(ItemName));
		}
		
	}
}

void UItemSlotWidget::OnButton_ItemSlotHovered()
{
	TextBlock_Use->SetVisibility(ESlateVisibility::Visible);
}

void UItemSlotWidget::OnButton_ItemSlotUnHovered()
{
	TextBlock_Use->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UItemSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	if (ItemName == NAME_None)
	{
		return FReply::Unhandled();
	}
	else
	{
		if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			return ReplyResult.NativeReply;
		}
	}
	return FReply::Unhandled();
}

void UItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UClass* DragItemWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/DDItem/WBP_DragItem.WBP_DragItem_C"));
	if (DragItemWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemName.ToString());
		DragItemWidget = CreateWidget<UDragItemWidget>(this, DragItemWidgetClass);
		if (DragItemWidget)
		{
			DragItemWidget->SetItemName(ItemName);
			DragItemWidget->SetQuantity(Quantity);
			DragItemWidget->UpdateDragItemWidget();
		
			UDDInventorySlot* DDInventorySlot = NewObject<UDDInventorySlot>(this);
			if (DDInventorySlot)
			{
				DDInventorySlot->DefaultDragVisual = DragItemWidget;
				DDInventorySlot->SetInventoryComponent(InventoryComponent);
				DDInventorySlot->SetContentIndex(Index);
				OutOperation = DDInventorySlot;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed DDInventorySlot"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed DragItemWidget"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed DragItemWidgetClass"));
	}
}

