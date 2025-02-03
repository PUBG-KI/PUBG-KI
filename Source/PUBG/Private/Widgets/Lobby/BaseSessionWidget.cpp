// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/BaseSessionWidget.h"
#include "Widgets/Lobby/BaseButtonWidget.h"

void UBaseSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Back->SetText(TEXT("Back"));	
}

void UBaseSessionWidget::OnBackButton_Clicked()
{
	OnBackButtonClicked.Broadcast();
}
