// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DisplayMessage/DisplayMessageItemWidget.h"

#include "Components/TextBlock.h"

void UDisplayMessageItemWidget::SetMessage(FText& NewText)
{
	Message = NewText;

	if (Message.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Text_DisplayMessageItem->SetText(Message);
		SetVisibility(ESlateVisibility::Visible);
	}
}
