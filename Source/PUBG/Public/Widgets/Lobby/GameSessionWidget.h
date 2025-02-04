// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "GameSessionWidget.generated.h"

class UBaseButtonWidget;

/**
 * 
 */
UCLASS()
class PUBG_API UGameSessionWidget : public UWidgetBase
{
	GENERATED_BODY()

	
public:		
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void OnStartButton_Clicked();
	
	UFUNCTION(BlueprintCallable)
	void OnReadyButton_Clicked();
	
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	
	UFUNCTION(BlueprintCallable)
	void OnQuitButton_Clicked();
	
	UFUNCTION(BlueprintCallable)
	void SetReady();	

private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UBaseButtonWidget* Button_Ready;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UBaseButtonWidget* Button_Quit;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bIsReady;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString LeaveMapName;
};
