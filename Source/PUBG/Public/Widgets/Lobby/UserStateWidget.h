// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "UserStateWidget.generated.h"

class UGridPanel;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class PUBG_API UUserStateWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:		
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetReadyState(bool IsReady);	
	
private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UGridPanel* Panel_Host;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UGridPanel* Panel_Player;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UGridPanel* Panel_ReadyState;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UWidgetSwitcher* WidgetSwitcher_State;		
};
