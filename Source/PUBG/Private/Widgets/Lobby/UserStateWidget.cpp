// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/UserStateWidget.h"

#include "Components/GridPanel.h"
#include "Components/WidgetSwitcher.h"

void UUserStateWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	Panel_ReadyState->SetVisibility(ESlateVisibility::Hidden);
}

void UUserStateWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerController* PC = GetOwningPlayer();
	if (PC->HasAuthority())
	{
		WidgetSwitcher_State->SetActiveWidget(Panel_Host);
	}
	else
	{
		WidgetSwitcher_State->SetActiveWidget(Panel_Player);
	}
}

void UUserStateWidget::SetReadyState(bool IsReady)
{
	if (IsReady == true)
	{
		Panel_ReadyState->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Panel_ReadyState->SetVisibility(ESlateVisibility::Hidden);
	}
}
