// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/GameSessionWidget.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Widgets/Lobby/BaseButtonWidget.h"


void UGameSessionWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Button_Quit->SetText(TEXT("Quit"));
}

void UGameSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	Button_Quit->OnButtonClicked.AddDynamic(this, &UGameSessionWidget::UGameSessionWidget::OnQuitButton_Clicked);

	APlayerController* PC = GetOwningPlayer();
	if (PC->HasAuthority())
	{
		Button_Ready->SetText(TEXT("Start"));
		Button_Ready->OnButtonClicked.AddDynamic(this, &UGameSessionWidget::UGameSessionWidget::OnStartButton_Clicked);
	}
	else
	{
		Button_Ready->SetText(TEXT("Ready"));
		Button_Ready->OnButtonClicked.AddDynamic(this, &UGameSessionWidget::UGameSessionWidget::OnReadyButton_Clicked);
	}
}

void UGameSessionWidget::OnStartButton_Clicked()
{
	if (GetOwningPlayer()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnStartButton_Clicked"));
	}
}

void UGameSessionWidget::OnReadyButton_Clicked()
{
	SetReady();
}

void UGameSessionWidget::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("OnDestroySessionComplete"));
        
		// 클라이언트를 로비나 메인 메뉴로 이동
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->ClientTravel(LeaveMapName, TRAVEL_Absolute);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionFailed"));
	}
}

void UGameSessionWidget::OnQuitButton_Clicked()
{
	if (IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			// 세션 제거

			// 클라이언트가 세션을 나가려고 요청
			//if (SessionInterface->GetSessionSettings(NAME_GameSession)->GetOwnerId() == SessionInterface->GetUniquePlayerId(0))
				// 호스트에서만 세션 종료 요청이 가능
			SessionInterface->DestroySession(NAME_GameSession, FOnDestroySessionCompleteDelegate::CreateUObject(this, &UGameSessionWidget::OnDestroySessionComplete));
			SessionInterface->DestroySession(NAME_GameSession, FOnDestroySessionCompleteDelegate::CreateUObject(this, &UGameSessionWidget::OnDestroySessionComplete));
		}
	}
}

void UGameSessionWidget::SetReady()
{

	GetOwningPlayerState();
	
	if (bIsReady)
		bIsReady = false;
	else
		bIsReady = true;
}


