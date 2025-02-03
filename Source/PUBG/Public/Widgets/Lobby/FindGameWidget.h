// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Lobby/BaseSessionWidget.h"
#include "FindGameWidget.generated.h"

class UBaseButtonWidget;
class UCheckBox;

/**
 * 
 */
UCLASS()
class PUBG_API UFindGameWidget : public UBaseSessionWidget
{
	GENERATED_BODY()
	
public:		
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void OnRefreshButton_Clicked();	
	UFUNCTION(BlueprintCallable)
	void OnCheckState_Changed(bool IsChecked);
	
private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UBaseButtonWidget* Button_Refresh;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UCheckBox* CheckBox_EnableLan;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UEditableText* Text_MaxPlayer;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int MaxPlayer;	
	
};
