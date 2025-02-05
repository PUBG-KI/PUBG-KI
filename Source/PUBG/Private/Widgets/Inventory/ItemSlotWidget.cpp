// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/ItemSlotWidget.h"

#include "BaseLibrary/DataStruct/ItemStruct.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "DragDrop/DDInventorySlot.h"
#include "Widgets/Inventory/DragItemWidget.h"
#include "Widgets/Inventory/InventoryWidget.h"

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

	VerticalBox_Near = InventoryWidget->GetVerticalBox_Near();
	VerticalBox_Inventory = InventoryWidget->GetVerticalBox_Inventory();
	
	//ItemZoneType = EItemZoneType::None;

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
			StartMousePoint = InMouseEvent;
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
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayer()->GetPawn());

	if (DragItemWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemName.ToString());
		DragItemWidget = CreateWidget<UDragItemWidget>(this, DragItemWidgetClass);
		if (DragItemWidget)
		{
			DragItemWidget->SetItemName(ItemName);
			DragItemWidget->SetQuantity(Quantity);
			DragItemWidget->UpdateDragItemWidget();
			DragItemWidget->SetInventoryWidget(InventoryWidget);
		
			UDDInventorySlot* DDInventorySlot = NewObject<UDDInventorySlot>(this);
			if (DDInventorySlot)
			{
				DDInventorySlot->DefaultDragVisual = DragItemWidget;
				
				if (InventoryComponent)
				{
					
					DDInventorySlot->SetInventoryComponent(InventoryComponent);
				}
				else if (NearComponent)
				{
					DDInventorySlot->SetNearComponent(NearComponent);
				}
				
				DDInventorySlot->SetContentIndex(Index);
				OutOperation = DDInventorySlot;


			// 드래그 시 특정 위젯 변수의 영역에 있는지 확인
				// 1. 시작한 영역인지 확인 (마우스의 시작 / 현재 지점이 어느 영역에 있는지 비교하여 진행)
				// if (CheckItemZoneType(StartMousePoint) == CheckItemZoneType(InMouseEvent))
				// {
				// 	UE_LOG(LogTemp, Warning, TEXT("Same Location"));
				// 	return;
				// }
				// // 2. 다르면 그 영역에 대한 UI 처리
				// else
				// {
				// 	ItemZoneType = CheckItemZoneType(InMouseEvent);
				// 	UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
				// 	if (ItemZoneType == EItemZoneType::NoneArea)
				// 	{
				// 		
				// 		return;
				// 	}
				// 	else if (ItemZoneType == EItemZoneType::NearArea)
				// 	{
				//
				// 		return;
				// 	}
				// 	else if (ItemZoneType == EItemZoneType::InventoryArea)
				// 	{
				//
				// 		return;
				// 	}
				// 	else if (ItemZoneType == EItemZoneType::EquippedArea)
				// 	{
				// 		// todo : UI 업데이트 
				// 		return;
				// 	}
				// }
				
				
			}
			// DDInventory 생성 실패
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed DDInventorySlot"));
			}
		// DragItemWidget 생성 실패
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed DragItemWidget"));
		}
	// DragItemWidgetClass 불러오기 실패 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed DragItemWidgetClass"));
	}
}

// bool UItemSlotWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
// 	UDragDropOperation* InOperation)
// {
// 	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
//
// 	ItemZoneType = CheckItemZoneType(InDragDropEvent);
// 	UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
// 	
// 	if (ItemZoneType == EItemZoneType::NoneArea)
// 	{
// 						
// 		return true;
// 	}
// 	else if (ItemZoneType == EItemZoneType::NearArea)
// 	{
//
// 		return true;
// 	}
// 	else if (ItemZoneType == EItemZoneType::InventoryArea)
// 	{
//
// 		return true;
// 	}
// 	else if (ItemZoneType == EItemZoneType::EquippedArea)
// 	{
// 		// todo : UI 업데이트 
// 		return true;
// 	}
//
// 	// if (CheckItemZoneType(StartMousePoint) == CheckItemZoneType(InDragDropEvent))
// 	// {
// 	// 	UE_LOG(LogTemp, Warning, TEXT("Same Location"));
// 	// 	return true;
// 	// }
// 	// // 2. 다르면 그 영역에 대한 UI 처리
// 	// else
// 	// {
// 	// 	ItemZoneType = CheckItemZoneType(InDragDropEvent);
// 	// 	UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
// 	// 	if (ItemZoneType == EItemZoneType::NoneArea)
// 	// 	{
// 	// 					
// 	// 		return true;
// 	// 	}
// 	// 	else if (ItemZoneType == EItemZoneType::NearArea)
// 	// 	{
// 	//
// 	// 		return true;
// 	// 	}
// 	// 	else if (ItemZoneType == EItemZoneType::InventoryArea)
// 	// 	{
// 	//
// 	// 		return true;
// 	// 	}
// 	// 	else if (ItemZoneType == EItemZoneType::EquippedArea)
// 	// 	{
// 	// 		// todo : UI 업데이트 
// 	// 		return true;
// 	// 	}
// 	// }
// 	//
// 	return false;
// }
//
// bool UItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
//                                    UDragDropOperation* InOperation)
// {
// 	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
//
// 	UDDInventorySlot* DDInventorySlot = Cast<UDDInventorySlot>(InOperation);
//
// 	if (!DDInventorySlot || ItemZoneType == EItemZoneType::None)
// 	{
// 		return false;
// 	}
// 	
// 	UE_LOG(LogTemp, Warning, TEXT("Drop ItemZoneType : %d"), ItemZoneType);
//
// 	if (ItemZoneType == EItemZoneType::NoneArea)
// 	{
// 		return true;
// 	}
// 	else if (ItemZoneType == EItemZoneType::NearArea)
// 	{
//
// 		return true;
// 	}
// 	else if (ItemZoneType == EItemZoneType::InventoryArea)
// 	{
//
// 		return true;
// 	}
// 	else if (ItemZoneType == EItemZoneType::EquippedArea)
// 	{
// 		// todo : UI 업데이트 
// 		return true;
// 	}
//
// 	return false;
// }
//
// EItemZoneType UItemSlotWidget::CheckItemZoneType(FPointerEvent InMousePoint)
// {
// 	if (VerticalBox_Near || VerticalBox_Inventory)
// 	{
// 		return EItemZoneType::None;
// 	}
// 	
// 	FVector2D  CurrentMouseSpace = InMousePoint.GetScreenSpacePosition();
//
// 	// 주변 위젯 사이즈
// 	FGeometry VerticalBox_NearGeometry = VerticalBox_Near->GetCachedGeometry();
// 	FVector2D VerticalBox_NearPos = VerticalBox_NearGeometry.GetAbsolutePosition();
// 	FVector2D VerticalBox_NearSize = VerticalBox_NearGeometry.GetLocalSize();
//
// 	// 인벤 위젯 사이즈
// 	FGeometry VerticalBox_InventoryGeometry = VerticalBox_Inventory->GetCachedGeometry();
// 	FVector2D VerticalBox_InventoryPos = VerticalBox_InventoryGeometry.GetAbsolutePosition();
// 	FVector2D VerticalBox_InventorySize = VerticalBox_InventoryGeometry.GetLocalSize();
//
// 	// todo : 장작된 장비 사이즈
//
// 	if (CurrentMouseSpace.X >= VerticalBox_NearPos.X && CurrentMouseSpace.X <= VerticalBox_NearPos.X + VerticalBox_NearSize.X &&
// 		CurrentMouseSpace.Y >= VerticalBox_NearPos.Y && CurrentMouseSpace.Y <= VerticalBox_NearPos.Y + VerticalBox_NearSize.Y)
// 	{
// 		return EItemZoneType::NearArea;
// 	}
// 	else if (CurrentMouseSpace.X >= VerticalBox_InventoryPos.X && CurrentMouseSpace.X <= VerticalBox_InventoryPos.X + VerticalBox_InventorySize.X &&
// 		CurrentMouseSpace.Y >= VerticalBox_InventoryPos.Y && CurrentMouseSpace.Y <= VerticalBox_InventoryPos.Y + VerticalBox_InventorySize.Y)
// 	{
// 		return EItemZoneType::InventoryArea;
// 	}
// 	// todo : 장착된 장비랑도 비교
// 	else
// 	{
// 		return EItemZoneType::NoneArea;
// 	}
// }
//
// EItemZoneType UItemSlotWidget::CheckItemZoneType(FDragDropEvent InDragDropEvent)
// {
// 	if (VerticalBox_Near || VerticalBox_Inventory)
// 	{
// 		return EItemZoneType::None;
// 	}
// 	
// 	FVector2D  CurrentMouseSpace = InDragDropEvent.GetScreenSpacePosition();
//
// 	// 주변 위젯 사이즈
// 	FGeometry VerticalBox_NearGeometry = VerticalBox_Near->GetCachedGeometry();
// 	FVector2D VerticalBox_NearPos = VerticalBox_NearGeometry.GetAbsolutePosition();
// 	FVector2D VerticalBox_NearSize = VerticalBox_NearGeometry.GetLocalSize();
//
// 	// 인벤 위젯 사이즈
// 	FGeometry VerticalBox_InventoryGeometry = VerticalBox_Inventory->GetCachedGeometry();
// 	FVector2D VerticalBox_InventoryPos = VerticalBox_InventoryGeometry.GetAbsolutePosition();
// 	FVector2D VerticalBox_InventorySize = VerticalBox_InventoryGeometry.GetLocalSize();
//
// 	// todo : 장작된 장비 사이즈
//
// 	if (CurrentMouseSpace.X >= VerticalBox_NearPos.X && CurrentMouseSpace.X <= VerticalBox_NearPos.X + VerticalBox_NearSize.X &&
// 		CurrentMouseSpace.Y >= VerticalBox_NearPos.Y && CurrentMouseSpace.Y <= VerticalBox_NearPos.Y + VerticalBox_NearSize.Y)
// 	{
// 		return EItemZoneType::NearArea;
// 	}
// 	else if (CurrentMouseSpace.X >= VerticalBox_InventoryPos.X && CurrentMouseSpace.X <= VerticalBox_InventoryPos.X + VerticalBox_InventorySize.X &&
// 		CurrentMouseSpace.Y >= VerticalBox_InventoryPos.Y && CurrentMouseSpace.Y <= VerticalBox_InventoryPos.Y + VerticalBox_InventorySize.Y)
// 	{
// 		return EItemZoneType::InventoryArea;
// 	}
// 	// todo : 장착된 장비랑도 비교
// 	else
// 	{
// 		return EItemZoneType::NoneArea;
// 	}
// }
