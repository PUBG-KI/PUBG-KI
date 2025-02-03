// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/ServerBrowserItemWidget.h"

#include "OnlineSessionSettings.h"
#include "Widgets/Lobby/BaseButtonWidget.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Components/TextBlock.h"
#include "Interfaces/OnlineSessionInterface.h"

void UServerBrowserItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Join->OnButtonClicked.AddDynamic(this, &UServerBrowserItemWidget::OnJoinButton_Clicked);
}

void UServerBrowserItemWidget::OnJoinButton_Clicked()
{
	if (IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString ServerName = TEXT("Unknown");			
			if (SearchResult.Session.SessionSettings.Get(FName("SESSION_NAME"), ServerName))
			{
				if (SessionInterface->JoinSession(0, *ServerName, SearchResult))
				{
					
				}
			}
		}
	}	
}

void UServerBrowserItemWidget::SetSession(const FOnlineSessionSearchResult& _SearchResult) const
{
	SearchResult = _SearchResult;
	
	// 서버 이름 가져오기
	FString ServerName = TEXT("Unknown");
	if (SearchResult.Session.SessionSettings.Get(FName("SESSION_NAME"), ServerName))
	{
		Text_SessionName->SetText(FText::FromString(ServerName));
	}

	// 현재 참가한 인원 계산
	int32 CurrentPlayers = SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections;
	// 최대 인원 가져오기
	int32 MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
	FString Player_Str = FString::FromInt(CurrentPlayers) + TEXT(" / ") + FString::FromInt(MaxPlayers) + TEXT("Players");
	Text_Players->SetText(FText::FromString(Player_Str));  
	
	// 핑 값 가져오기
	int32 Ping = SearchResult.PingInMs;	
	FString Ping_Str = FString::FromInt(Ping) + TEXT("m/s");
	Text_Players->SetText(FText::FromString(Ping_Str));  
}
