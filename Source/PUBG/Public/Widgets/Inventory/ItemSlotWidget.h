// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "ItemSlotWidget.generated.h"

class UInventoryComponent;
class UButton;
class UImage;
class UTextBlock;
class UDataTable;
class UWrapBox;
class UDragItemWidget;
/**
 * 
 */
UCLASS()
class PUBG_API UItemSlotWidget : public UWidgetBase
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UButton* Button_ItemSlot;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_Icon;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_ItemName;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_Quantity;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* TextBlock_Use;

	UPROPERTY()
	FName ItemName;
	UPROPERTY()
	int32 Quantity;
	UPROPERTY()
	UInventoryComponent* InventoryComponent;
	UPROPERTY()
	int32 Index;

	UPROPERTY()
	UDragItemWidget* DragItemWidget;
	
	// UPROPERTY()
	// UDataTable* DataTable;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	//Setter
	void SetItemName(FName OutItemName) { ItemName = OutItemName; }
	void SetQuantity(int32 OutQuantity) { Quantity = OutQuantity; }
	void SetInventoryComponent(UInventoryComponent* OutInventoryComponent) { InventoryComponent = OutInventoryComponent; }
	void SetIndex(int32 OutIndex) { Index = OutIndex; }

	UFUNCTION(BlueprintCallable)
	void UpdateItemSlotWidget();

	UFUNCTION(BlueprintCallable)
	void OnButton_ItemSlotHovered();
	UFUNCTION(BlueprintCallable)
	void OnButton_ItemSlotUnHovered();

	
	
	virtual FReply NativeOnPreviewMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation ) override;
};
