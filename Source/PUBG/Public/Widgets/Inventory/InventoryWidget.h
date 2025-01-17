// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Widgets/WidgetBase.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */

class UItemSlotWidget;
class UWrapBox;

UCLASS()
class PUBG_API UInventoryWidget : public UWidgetBase
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere)
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere)
	UItemSlotWidget* ItemSlotWidget;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox_Inventory;


public:
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateInventoryWidget();
	UFUNCTION(BlueprintCallable)
	void AddNewChild();

	//Setter
	void SetInventoryComponent(UInventoryComponent* OutInventoryComponent) { InventoryComponent = OutInventoryComponent; }

	//Getter
	UItemSlotWidget* GetItemSlotWidget() const { return ItemSlotWidget; }
	UWrapBox *GetWrapBox_Inventory() const { return WrapBox_Inventory;}
};
