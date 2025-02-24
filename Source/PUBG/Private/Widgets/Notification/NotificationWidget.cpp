// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Notification/NotificationWidget.h"
#include "Components/TextBlock.h"


void UNotificationWidget::PlayWidgetAnimation()
{
	if (NotifyAnimation)
	{
		StopAnimation(NotifyAnimation);  // 애니메이션 중지
		PlayAnimation(NotifyAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	}
}

void UNotificationWidget::ShowNotification(FText Message)
{
	Text_Notification->SetText(Message);
	PlayWidgetAnimation();
}

void UNotificationWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
