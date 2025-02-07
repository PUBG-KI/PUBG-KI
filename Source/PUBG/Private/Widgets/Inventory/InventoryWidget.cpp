// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryWidget.h"

#include "Component/ItemData/ItemDataComponent.h"
#include "Component/NearArea/NearComponent.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WrapBox.h"
#include "Controller/BasePlayerController.h"
#include "DragDrop/DDInventorySlot.h"
#include "Item/ItemBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Inventory/ItemSlotWidget.h"


UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InventoryComponent = nullptr;
	ItemSlotWidget  = nullptr;
	WrapBox_Inventory  = nullptr;

	ItemSlotWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_ItemSlot.WBP_ItemSlot_C"));

	ItemZoneType = EItemZoneType::None;

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
	
	if (!GetWrapBox_Inventory())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWrapBox_Inventory() is NULL"));
		return;
	}
	
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
				ItemSlotWidget->SetInventoryWidget(this);
				
				FString text = TEXT("사용");
				ItemSlotWidget->GetTextBlock_Use()->SetText(FText::FromString(text));
				
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

void UInventoryWidget::UpdateNearItemSlotWidget()
{
	GetWrapBox_Near()->ClearChildren();
	//NearComponent->ServerGetGroundItem();
	TArray<AItemBase*> ItemSlot = NearComponent->GetGroundItems();
	
	UE_LOG(LogTemp, Warning, TEXT("ItemSlot Num : %d"), ItemSlot.Num());
	//UE_LOG(LogTemp, Warning, TEXT("GroundItem[0] : %s"), *NearComponent->GetGroundItems()[0]->GetItemStruct().Name.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("ItemSlot : %d"), ItemSlot.Num());
	
	for (int i = 0; i < ItemSlot.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ItemSlot Widget not found"));
		if (ItemSlotWidgetClass)
		{
			NearItemSlotWidget = CreateWidget<UItemSlotWidget>(GetWorld(), ItemSlotWidgetClass);
			if (NearItemSlotWidget != nullptr)
			{
				if (ItemSlot[i] != nullptr)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Name : %s"), *ItemSlot[i]->GetItemStruct().Name.ToString());
					UE_LOG(LogTemp, Warning, TEXT("Name : %s"), *ItemSlot[i]->GetItemDataComponent()->GetItemRowName().ToString());
					//NearItemSlotWidget->SetItemName(ItemSlot[i]->GetItemStruct().Name);
					NearItemSlotWidget->SetItemName(ItemSlot[i]->GetItemDataComponent()->GetItemRowName());
					//NearItemSlotWidget->SetQuantity(ItemSlot[i]->GetItemStruct().Quantity);
					NearItemSlotWidget->SetQuantity(ItemSlot[i]->GetItemDataComponent()->GetQuantity());
					NearItemSlotWidget->SetNearComponent(NearComponent);
					//NearItemSlotWidget->SetInventoryComponent(InventoryComponent);
					NearItemSlotWidget->SetIndex(i);
					NearItemSlotWidget->SetInventoryWidget(this);
					
					FString text = TEXT("줍기");
					NearItemSlotWidget->GetTextBlock_Use()->SetText(FText::FromString(text));
					//NearItemSlotWidget->GetTextBlock_Use()->SetText(LOCTEXT("UseText", "사용"));
					
					NearItemSlotWidget->UpdateItemSlotWidget();
					GetWrapBox_Near()->AddChildToWrapBox(NearItemSlotWidget);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemSlot Widget not found"));
		}
	}
	
	ItemSlot.Empty();
}

void UInventoryWidget::AddNewChild()
{
	GetWrapBox_Inventory()->AddChildToWrapBox(ItemSlotWidget);
	
}

bool UInventoryWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	UDDInventorySlot* DDInventorySlot = Cast<UDDInventorySlot>(InOperation);
	ItemZoneType = CheckItemZoneType(InDragDropEvent);
	
	if (!DDInventorySlot || ItemZoneType == EItemZoneType::None)
	{
		return false;
	}
	
	if (ItemZoneType == EItemZoneType::NoneArea)
	{
					
		//UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
		return true;
	}
	else if (ItemZoneType == EItemZoneType::NearArea && !DDInventorySlot->GetNearComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
		return true;
	}
	else if (ItemZoneType == EItemZoneType::InventoryArea && !DDInventorySlot->GetInventoryComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
		return true;
	}
	else if (ItemZoneType == EItemZoneType::EquippedArea)
	{
		// todo : UI 업데이트 
		return true;
	}
	// if (CheckItemZoneType(StartMousePoint) == CheckItemZoneType(InDragDropEvent))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Same Location"));
	// 	return true;
	// }
	// // 2. 다르면 그 영역에 대한 UI 처리
	// else
	// {
	// 	ItemZoneType = CheckItemZoneType(InDragDropEvent);
	// 	UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
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
	// }
	//
	return false;

	
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	UDDInventorySlot* DDInventorySlot = Cast<UDDInventorySlot>(InOperation);
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayer()->GetPawn());
	
	if (!DDInventorySlot || ItemZoneType == EItemZoneType::None || !PlayerCharacter)
	{
		return false;
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("Drop ItemZoneType : %d"), ItemZoneType);
	
	if (ItemZoneType == EItemZoneType::NoneArea)
	{
					
		//UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
		return true;
	}
	else if (ItemZoneType == EItemZoneType::NearArea && !DDInventorySlot->GetNearComponent()) // 놓는 위치 확인 및 시작 위치 컴포넌트 확인
	{
		UE_LOG(LogTemp, Warning, TEXT("Drop ItemZoneType : %d"), ItemZoneType);
		
		
		// DDInventorySlot에 담긴 값을 NearComponent에 추가
		switch (DDInventorySlot->GetHaveComponent())
		{
		case 2: // 시작이 인벤토리
			return true;
		case 3: // 시작이 장작된 장비
			return true;
			
		}
		
		return true;
	}
	else if (ItemZoneType == EItemZoneType::InventoryArea && !DDInventorySlot->GetInventoryComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("Drop ItemZoneType : %d"), ItemZoneType);
		// DDInventorySlot에 담긴 값을 InventoryComponent에 추가
		switch (DDInventorySlot->GetHaveComponent())
		{
		case 1: // 시작이 주위 도착이 인벤
			UE_LOG(LogTemp, Warning, TEXT("Start : Near, End : Inventory"));
			
			//PlayerCharacter->GetInventoryComponent()->SetNearItem(DDInventorySlot->GetNearComponent()->GetGroundItems()[DDInventorySlot->GetContentIndex()]);
			PlayerCharacter->GetInventoryComponent()->ServerSetNearItem(DDInventorySlot->GetNearComponent()->GetGroundItems()[DDInventorySlot->GetContentIndex()]);
			//PlayerCharacter->GetInventoryComponent()->SetItem(DDInventorySlot->GetNearComponent()->GetGroundItems()[DDInventorySlot->GetContentIndex()]);

			//UE_LOG(LogTemp, Warning, TEXT("Drop Item : %s"), *PlayerCharacter->GetInventoryComponent()->GetNearItem()->GetItemStruct().Name.ToString());
			
			//PlayerCharacter->GetInventoryComponent()->Server_Interact();
			PlayerCharacter->GetInventoryComponent()->Server_InteractItem(PlayerCharacter->GetInventoryComponent()->GetNearItem());
			
			if (ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetOwningPlayer()))
			{
				PlayerController->GetInventoryWidget()->UpdateInventoryWidget();
			}

			PlayerCharacter->GetInventoryComponent()->SetNearItem(nullptr);
			
			return true;
		case 3: // 시작이 장작된 장비 
			return true;
		}
		
		return true;
	}
	else if (ItemZoneType == EItemZoneType::EquippedArea)
	{
		// todo : UI 업데이트 
		return true;
	}
	
	return false;
	
}

EItemZoneType UInventoryWidget::CheckItemZoneType(FPointerEvent InMousePoint)
{
	if (VerticalBox_Near || VerticalBox_Inventory)
	{
		return EItemZoneType::None;
	}
	
	FVector2D  CurrentMouseSpace = InMousePoint.GetScreenSpacePosition();

	// 주변 위젯 사이즈
	FGeometry VerticalBox_NearGeometry = VerticalBox_Near->GetCachedGeometry();
	FVector2D VerticalBox_NearPos = VerticalBox_NearGeometry.GetAbsolutePosition();
	FVector2D VerticalBox_NearSize = VerticalBox_NearGeometry.GetLocalSize();

	// 인벤 위젯 사이즈
	FGeometry VerticalBox_InventoryGeometry = VerticalBox_Inventory->GetCachedGeometry();
	FVector2D VerticalBox_InventoryPos = VerticalBox_InventoryGeometry.GetAbsolutePosition();
	FVector2D VerticalBox_InventorySize = VerticalBox_InventoryGeometry.GetLocalSize();

	// todo : 장작된 장비 사이즈

	if (CurrentMouseSpace.X >= VerticalBox_NearPos.X && CurrentMouseSpace.X <= VerticalBox_NearPos.X + VerticalBox_NearSize.X &&
		CurrentMouseSpace.Y >= VerticalBox_NearPos.Y && CurrentMouseSpace.Y <= VerticalBox_NearPos.Y + VerticalBox_NearSize.Y)
	{
		return EItemZoneType::NearArea;
	}
	else if (CurrentMouseSpace.X >= VerticalBox_InventoryPos.X && CurrentMouseSpace.X <= VerticalBox_InventoryPos.X + VerticalBox_InventorySize.X &&
		CurrentMouseSpace.Y >= VerticalBox_InventoryPos.Y && CurrentMouseSpace.Y <= VerticalBox_InventoryPos.Y + VerticalBox_InventorySize.Y)
	{
		return EItemZoneType::InventoryArea;
	}
	// todo : 장착된 장비랑도 비교
	else
	{
		return EItemZoneType::NoneArea;
	}
}

EItemZoneType UInventoryWidget::CheckItemZoneType(FDragDropEvent InDragDropEvent)
{
	if (!VerticalBox_Near || !VerticalBox_Inventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("VerticalBox None"));
		return EItemZoneType::None;
	}

	// 커서 위치 확인
	FVector2D  CurrentMouseSpace = InDragDropEvent.GetScreenSpacePosition();

	// 주변 위젯 사이즈
	FGeometry VerticalBox_NearGeometry = VerticalBox_Near->GetCachedGeometry();
	FVector2D VerticalBox_NearPos = VerticalBox_NearGeometry.GetAbsolutePosition();
	//UE_LOG(LogTemp, Warning, TEXT("NearPos : %f %f"), VerticalBox_NearPos.X, VerticalBox_NearPos.Y);
	FVector2D VerticalBox_NearSize = VerticalBox_NearGeometry.GetAbsoluteSize();
	//UE_LOG(LogTemp, Warning, TEXT("NearSize : %f %f"), VerticalBox_NearSize.X, VerticalBox_NearSize.Y);

	// 인벤 위젯 사이즈
	FGeometry VerticalBox_InventoryGeometry = VerticalBox_Inventory->GetCachedGeometry();
	FVector2D VerticalBox_InventoryPos = VerticalBox_InventoryGeometry.GetAbsolutePosition();
	//UE_LOG(LogTemp, Warning, TEXT("InventoryPos : %f %f"), VerticalBox_InventoryPos.X, VerticalBox_InventoryPos.Y);
	FVector2D VerticalBox_InventorySize = VerticalBox_InventoryGeometry.GetAbsoluteSize();

	// todo : 장작된 장비 사이즈

	
	if (CurrentMouseSpace.X >= VerticalBox_NearPos.X && CurrentMouseSpace.X <= VerticalBox_NearPos.X + VerticalBox_NearSize.X &&
		CurrentMouseSpace.Y >= VerticalBox_NearPos.Y && CurrentMouseSpace.Y <= VerticalBox_NearPos.Y + VerticalBox_NearSize.Y)
	{
		return EItemZoneType::NearArea;
	}
	else if (CurrentMouseSpace.X >= VerticalBox_InventoryPos.X && CurrentMouseSpace.X <= VerticalBox_InventoryPos.X + VerticalBox_InventorySize.X &&
		CurrentMouseSpace.Y >= VerticalBox_InventoryPos.Y && CurrentMouseSpace.Y <= VerticalBox_InventoryPos.Y + VerticalBox_InventorySize.Y)
	{
		return EItemZoneType::InventoryArea;
	}
	// todo : 장착된 장비랑도 비교
	else
	{
		return EItemZoneType::NoneArea;
	}
}
