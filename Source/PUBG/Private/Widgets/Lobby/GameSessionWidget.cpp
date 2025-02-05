// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/GameSessionWidget.h"
#include "Components/HorizontalBox.h"
#include "Widgets/Lobby/BaseButtonWidget.h"
#include "Widgets/Lobby/UserStateWidget.h"

#include "GameState/LobbyGameState.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "GameMode/LobbyGameMode.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PlayerState/LobbyPlayerState.h"


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
		
		if (APlayerController* PC = GetOwningPlayer())
		{
			if (ALobbyGameMode* GM = Cast<ALobbyGameMode>(PC->GetWorld()->GetAuthGameMode()))
			{
				//GM->ServerStartGame();
			}
		}
	}
}

void UGameSessionWidget::OnReadyButton_Clicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ALobbyPlayerState* PS = PC->GetPlayerState<ALobbyPlayerState>())
		{
			PS->ServerSetReady(!PS->bIsReady);
		}
	}
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

void UGameSessionWidget::UpdatePlayerList(TArray<ALobbyPlayerState*> PlayerList)
{
	APlayerController* PC = GetOwningPlayer();
	
	UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerList"));
	HorizontalBox_PlayerList->ClearChildren(); // 기존 리스트 제거
	for (ALobbyPlayerState* PS : PlayerList)
	{
		UUserStateWidget* UserStateWidget = CreateWidget<UUserStateWidget>(PC, UserStateWidgetClass);
		UserStateWidget->InitializeUserState(PS);
		HorizontalBox_PlayerList->AddChild(UserStateWidget);
	}
}




