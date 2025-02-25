// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "DisplayMessageItemWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PUBG_API UDisplayMessageItemWidget : public UWidgetBase
{
	GENERATED_BODY()

private:
	// Widget Variable
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UTextBlock* Text_DisplayMessageItem;

	// Variable
	UPROPERTY()
	FText Message;

public:
	// Getter
	FText& GetMessage() { return Message; }

	// Setter
	void SetMessage(FText& NewText);
	
};
