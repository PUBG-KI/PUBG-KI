// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "DragItemWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PUBG_API UDragItemWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

	
private:
	UPROPERTY(meta = (BindWidget))
	UImage* Image_ItemImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_ItemQuantity;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName ItemName;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 Quantity;

public:
	UFUNCTION(BlueprintCallable)
	void SetQuantity(int32 OutQuantity) { Quantity = OutQuantity; }
	UFUNCTION(BlueprintCallable)
	void SetItemName(FName OutItemName) { ItemName = OutItemName; }

	UFUNCTION(BlueprintCallable)
	void UpdateDragItemWidget();
};
