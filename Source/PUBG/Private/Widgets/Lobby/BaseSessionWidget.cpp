// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/BaseSessionWidget.h"

void UBaseSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UBaseSessionWidget::OnBackButton_Clicked()
{
	OnBackButtonClicked.Broadcast();
}
