// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Component/Equipped/EquippedComponent.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "Component/NearArea/NearComponent.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WrapBox.h"
#include "Controller/BasePlayerController.h"
#include "DragDrop/DDInventorySlot.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Item/ItemBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Inventory/ItemSlotWidget.h"
#include "Widgets/Inventory/WeaponSlotWidget.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"


UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InventoryComponent = nullptr;
	ItemSlotWidget  = nullptr;
	WrapBox_Inventory  = nullptr;

	ItemSlotWidgetBPClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_ItemSlot.WBP_ItemSlot_C"));
	WeaponSlotWidgetBPClass1 = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_WeaponSlot1.WBP_WeaponSlot1_C"));
	WeaponSlotWidgetBPClass2 = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_WeaponSlot2.WBP_WeaponSlot2_C"));
	WeaponSlotWidgetBPClass3 = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_WeaponSlot3.WBP_WeaponSlot3_C"));
	
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
		if (ItemSlotWidgetBPClass)
		{
			ItemSlotWidget = CreateWidget<UItemSlotWidget>(GetWorld(), ItemSlotWidgetBPClass);
			if (ItemSlotWidget != nullptr)
			{
				ItemSlotWidget->SetItemName(ItemSlot[i].ItemName);
				//ItemSlotWidget->GetItemName
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
		if (ItemSlotWidgetBPClass)
		{
			NearItemSlotWidget = CreateWidget<UItemSlotWidget>(GetWorld(), ItemSlotWidgetBPClass);
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

void UInventoryWidget::UpdateEquippedWidget()
{
	if (!WeaponSlotWidgetBPClass1 &&!WeaponSlotWidgetBPClass2 && !WeaponSlotWidgetBPClass3 && !EquippedComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponSlotWidgetClass None Or EquippedComponent None"));
		return;
	}
		
	TArray<AEquipableItem*> EquippedItems = EquippedComponent->GetEquippedItems();

	UE_LOG(LogTemp, Warning, TEXT("EquippedItems Num : %d"), EquippedItems.Num());

	EquippedUIInit();
	
	for (int i = 0; i < EquippedItems.Num(); i++)
	{
		if (EquippedItems[i] != nullptr)
		{
			switch (i)
			{
			case 0:
				Weapon1SlotWidget = CreateWidget<UWeaponSlotWidget>(GetWorld(), WeaponSlotWidgetBPClass1);
				
				if (Weapon1SlotWidget)
				{	
					AGun_Base* Slot1Weapon = Cast<AGun_Base>(EquippedItems[i]); // GunBase로 캐스팅
					Weapon1SlotWidget = EquippedWeaponUIUpdate(Weapon1SlotWidget, Slot1Weapon, i);

					SizeBox_1Slot->SetContent(Weapon1SlotWidget);
					UE_LOG(LogTemp, Warning, TEXT("%d : SetContent"), i);
					
					// Weapon1SlotWidget->FindEquiablePartsSlot(Slot1Weapon);
					//
					// FString SlotNumber =  FString::FromInt(i + 1);
					// Weapon1SlotWidget->GetTextSlotNumber()->SetText(FText::FromString(SlotNumber)); // 슬롯 번호 지정 
					//
					// FString WeaponName = Slot1Weapon->GetWeaponDataAsset().GunName;
					// Weapon1SlotWidget->GetTextWeaponName()->SetText(FText::FromString(WeaponName)); // 총 이름 지정 
					//
					//
					// // 장전된 총알, 남은 총알 
					//
					// EBulletType WeaponBulletType = Slot1Weapon->GetWeaponDataAsset().BulletType;
					// FString WeaponBulletName = Weapon1SlotWidget->SetBulletTypeTextBlock(WeaponBulletType);
					// Weapon1SlotWidget->GetTextAmmoName()->SetText(FText::FromString(WeaponBulletName)); // 총알 이름 지정
					//
					// FName WeaponBulletFName = FName(*WeaponBulletName);
					// FItemStruct* Row = Slot1Weapon->GetItemDataTable()->FindRow<FItemStruct>(WeaponBulletFName, TEXT("Find Row"));
					// Weapon1SlotWidget->GetImageAmmoImage()->SetRenderTranslation(FVector2D(-5.0f, 0.0f));
					// Weapon1SlotWidget->GetImageAmmoImage()->SetRenderScale(FVector2D(2.0f, 2.0f));
					// Weapon1SlotWidget->GetImageAmmoImage()->SetBrushFromTexture(Row->Image); // 총알 이미지 지정 
					//
					//
					// SizeBox_1Slot->SetContent(Weapon1SlotWidget);
					// UE_LOG(LogTemp, Warning, TEXT("%d : SetContent"), i);
				}
				break;
			case 1:
				Weapon2SlotWidget = CreateWidget<UWeaponSlotWidget>(GetWorld(), WeaponSlotWidgetBPClass2);
				
				if (Weapon2SlotWidget)
				{
					AGun_Base* Slot2Weapon = Cast<AGun_Base>(EquippedItems[i]); // GunBase로 캐스팅
					Weapon2SlotWidget = EquippedWeaponUIUpdate(Weapon2SlotWidget, Slot2Weapon, i);

					SizeBox_2Slot->SetContent(Weapon2SlotWidget);
					UE_LOG(LogTemp, Warning, TEXT("%d : SetContent"), i);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Weapon2SlotWidget None"));
				}
				break;
			case 2:
				Weapon3SlotWidget = CreateWidget<UWeaponSlotWidget>(GetWorld(), WeaponSlotWidgetBPClass3);
				
				if (Weapon3SlotWidget)
				{
					AGun_Base* Slot3Weapon = Cast<AGun_Base>(EquippedItems[i]); // GunBase로 캐스팅
					Weapon3SlotWidget = EquippedWeaponUIUpdate(Weapon3SlotWidget, Slot3Weapon, i);

					SizeBox_3Slot->SetContent(Weapon3SlotWidget);
					UE_LOG(LogTemp, Warning, TEXT("%d : SetContent"), i);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Weapon3SlotWidget None"));
				}
				break;
			}
		}
	}
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
		case 2: // 시작이 인벤토리 도착이 바닥
			UE_LOG(LogTemp, Warning, TEXT("Start : Inventory, End : Near"));
			PlayerCharacter->GetInventoryComponent()->RemoveFromInventory(DDInventorySlot->GetContentIndex(),
																			false,
																			DDInventorySlot->GetInventoryComponent()->GetContent()[DDInventorySlot->GetContentIndex()].Quantity);
			return true;
		case 3: // 시작이 장작된 장비 도착이 바닥 
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
			
			PlayerCharacter->GetInventoryComponent()->ServerSetNearItem(DDInventorySlot->GetNearComponent()->GetGroundItems()[DDInventorySlot->GetContentIndex()]);
			
			PlayerCharacter->GetInventoryComponent()->Server_InteractItem(PlayerCharacter->GetInventoryComponent()->GetNearItem());
			
			if (ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetOwningPlayer()))
			{
				PlayerController->GetInventoryWidget()->UpdateInventoryWidget();
			}

			PlayerCharacter->GetInventoryComponent()->SetNearItem(nullptr);
			
			return true;
		case 3: // 시작이 장작된 장비 도착이 인벤 
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

void UInventoryWidget::EquippedUIInit()
{
	if (Weapon1SlotWidget != nullptr)
	{
		Weapon1SlotWidget->RemoveFromParent();
		Weapon1SlotWidget = nullptr;
	}
	

	if (Weapon2SlotWidget != nullptr)
	{
		Weapon2SlotWidget->RemoveFromParent();
		Weapon2SlotWidget = nullptr;
	}

	if (Weapon3SlotWidget != nullptr)
	{
		Weapon3SlotWidget->RemoveFromParent();
		Weapon3SlotWidget = nullptr;
	}
}

UWeaponSlotWidget* UInventoryWidget::EquippedWeaponUIUpdate(UWeaponSlotWidget* OutWeaponSlotWidget, AGun_Base* OutGunBase, int32 OutIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::EquippedWeaponUIUpdate"));					
	OutWeaponSlotWidget->FindEquiablePartsSlot(OutGunBase);

	FString SlotNumber =  FString::FromInt(OutIndex + 1);
	OutWeaponSlotWidget->GetTextSlotNumber()->SetText(FText::FromString(SlotNumber)); // 슬롯 번호 지정 
					
	FString WeaponName = OutGunBase->GetWeaponDataAsset().GunName;
	OutWeaponSlotWidget->GetTextWeaponName()->SetText(FText::FromString(WeaponName)); // 총 이름 지정 


	// 장전된 총알, 남은 총알 
					
	EBulletType WeaponBulletType = OutGunBase->GetWeaponDataAsset().BulletType;
	FString WeaponBulletName = OutWeaponSlotWidget->SetBulletTypeTextBlock(WeaponBulletType);
	OutWeaponSlotWidget->GetTextAmmoName()->SetText(FText::FromString(WeaponBulletName)); // 총알 이름 지정

	FName WeaponBulletFName = FName(*WeaponBulletName);
	UE_LOG(LogTemp, Warning, TEXT("EquippedWeaponUIUpdate : %s"), *WeaponBulletName);
	FItemStruct* Row = OutGunBase->GetItemDataTable()->FindRow<FItemStruct>(WeaponBulletFName, TEXT("Find Row"));
	
	if (Row != nullptr)
	{
		OutWeaponSlotWidget->GetImageAmmoImage()->SetRenderTranslation(FVector2D(-5.0f, 0.0f));
		OutWeaponSlotWidget->GetImageAmmoImage()->SetRenderScale(FVector2D(2.0f, 2.0f));
		OutWeaponSlotWidget->GetImageAmmoImage()->SetBrushFromTexture(Row->Image); // 총알 이미지 지정 
	}

	return OutWeaponSlotWidget;
}
