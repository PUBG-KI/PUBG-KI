// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "BaseSessionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackButton_Clicked);

class UBaseButtonWidget;
/**
 * 
 */
UCLASS()
class PUBG_API UBaseSessionWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void OnBackButton_Clicked();
	
private:	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UBaseButtonWidget* Button_Back;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnBackButton_Clicked OnBackButtonClicked;
};
