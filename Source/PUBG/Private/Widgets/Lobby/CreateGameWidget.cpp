// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/CreateGameWidget.h"

#include "Components/CheckBox.h"
#include "Components/EditableText.h"
#include "Widgets/Lobby/BaseButtonWidget.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"

void UCreateGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Create->OnButtonClicked.AddDynamic(this, &UCreateGameWidget::OnCreateButton_Clicked);
	Text_MaxPlayer->OnTextChanged.AddDynamic(this, &UCreateGameWidget::OnText_Changed);
	CheckBox_EnableLan->OnCheckStateChanged.AddDynamic(this, &UCreateGameWidget::OnCheckState_Changed);
}

void UCreateGameWidget::OnCreateButton_Clicked()
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsLANMatch = bIsUseLan;  // LAN 테스트용 (Steam/EOS는 false)
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bUsesPresence = true;
			SessionSettings.NumPublicConnections = MaxPlayer;
			SessionSettings.bAllowJoinInProgress = true;

			SessionInterface->CreateSession(0, FName("MyGameSession"), SessionSettings);
		}
	}
}

void UCreateGameWidget::OnText_Changed(const FText& Text)
{
	FString Temp = Text.ToString();
	
	if (Temp.IsNumeric())
	{
		MaxPlayer = FCString::Atoi(*Temp);
	}
}

void UCreateGameWidget::OnCheckState_Changed(bool IsChecked)
{
	bIsUseLan = IsChecked;
}

