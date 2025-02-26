// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Result/ResultWidget.h"

#include "Components/TextBlock.h"
#include "Widgets/Lobby/BaseButtonWidget.h"


UResultWidget::UResultWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UResultWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();	
	WBP_ResultButton->OnButtonClicked.AddDynamic(this, &UResultWidget::OnResultButton_Clicked);
}

void UResultWidget::SetText_ResultMessage(FString Text)
{
	Text_ResultMessage->SetText(FText::FromString(Text));
}

void UResultWidget::SetText_Rank(FString Text)
{
	FText Num = FText::FromString(Text);
	Text_Rank_1->SetText(Num);
	Text_Rank_2->SetText(Num);
}

void UResultWidget::SetText_PlayerNum(FString Text)
{
	Text_PlayerNum->SetText(FText::FromString(Text));
}

void UResultWidget::SetText_Kill(FString Text)
{
	Text_Kill->SetText(FText::FromString(Text));
}

void UResultWidget::OnResultButton_Clicked()
{
	
}
