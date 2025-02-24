// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "NotificationWidget.generated.h"

class UWidgetAnimation;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PUBG_API UNotificationWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	// 애니메이션을 찾아서 재생하는 함수 선언
	UFUNCTION(BlueprintCallable)
	void PlayWidgetAnimation();
	UFUNCTION(BlueprintCallable)
	void ShowNotification(FText Message);

protected:
	virtual void NativeConstruct() override;

	// 블루프린트에서 애니메이션을 가져오기 위한 변수
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* NotifyAnimation;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Notification;

	
};
