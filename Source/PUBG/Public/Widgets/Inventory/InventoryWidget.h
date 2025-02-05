// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Widgets/WidgetBase.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */

class UVerticalBox;
class UNearComponent;
class UItemSlotWidget;
class UWrapBox;

UCLASS()
class PUBG_API UInventoryWidget : public UWidgetBase
{
	GENERATED_BODY()


private:
	// Variable
	UPROPERTY(EditAnywhere)
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere)
	UNearComponent* NearComponent;
	
	UPROPERTY(EditAnywhere)
	UItemSlotWidget* ItemSlotWidget;
	UPROPERTY(EditAnywhere)
	UItemSlotWidget* NearItemSlotWidget;

	UPROPERTY()
	EItemZoneType ItemZoneType;

	// Widget Variable
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox_Inventory;
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox_Near;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox_Near;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox_Inventory;

	UClass* ItemSlotWidgetClass;

public:
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateInventoryWidget();
	UFUNCTION(BlueprintCallable)
	void UpdateNearItemSlotWidget();
	UFUNCTION(BlueprintCallable)
	void AddNewChild();

	//Setter
	void SetInventoryComponent(UInventoryComponent* OutInventoryComponent) { InventoryComponent = OutInventoryComponent; }
	void SetNearComponent(UNearComponent* OutNearComponent) { NearComponent = OutNearComponent; }

	//Getter
	UItemSlotWidget* GetItemSlotWidget() const { return ItemSlotWidget; }
	UWrapBox *GetWrapBox_Inventory() const { return WrapBox_Inventory; }
	UWrapBox *GetWrapBox_Near() const { return WrapBox_Near; }
	UVerticalBox* GetVerticalBox_Inventory() const { return VerticalBox_Inventory; }
	UVerticalBox* GetVerticalBox_Near() const { return VerticalBox_Near; }

	virtual bool NativeOnDragOver( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;

	// 드래그 시 영역 확인
	EItemZoneType CheckItemZoneType(FPointerEvent InMousePoint);
	EItemZoneType CheckItemZoneType(FDragDropEvent InDragDropEvent);
};
