// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "ServerBrowserItemWidget.generated.h"

class UBaseButtonWidget;
class UTextBlock;
class FOnlineSessionSearchResult;

/**
 * 
 */
UCLASS()
class PUBG_API UServerBrowserItemWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void OnJoinButton_Clicked();
	
	//UFUNCTION(BlueprintCallable)
	void SetSession(const FOnlineSessionSearchResult& _SearchResult) const;	
	
private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UBaseButtonWidget* Button_Join;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_SessionName;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_Players;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_Ping;
	
	FOnlineSessionSearchResult& SearchResult;
};
