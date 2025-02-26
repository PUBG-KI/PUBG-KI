// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "ResultWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PUBG_API UResultWidget : public UWidgetBase
{
	GENERATED_BODY()
	
	
public:	
	UResultWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetText_ResultMessage(FString Text);
	UFUNCTION(BlueprintCallable)
	void SetText_Rank(FString Text);
	UFUNCTION(BlueprintCallable)
	void SetText_PlayerNum(FString Text);
	UFUNCTION(BlueprintCallable)
	void SetText_Kill(FString Text);
	UFUNCTION(BlueprintCallable)
	void OnResultButton_Clicked();	
	
private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_ResultMessage;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_Rank_1;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_PlayerNum;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_Rank_2;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_Kill;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UBaseButtonWidget* WBP_ResultButton;
	
};
