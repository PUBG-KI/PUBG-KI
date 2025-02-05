// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/LobbyPlayerController.h"

#include "GameState/LobbyGameState.h"
#include "Widgets/Lobby/GameSessionWidget.h"


void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())  // 로컬 플레이어만 UI 표시
	{
		ShowLobby();
		UpdateWidget();
	}
}

void ALobbyPlayerController::ShowLobby()
{
	if (LobbyWidgetClass)  // 위젯 블루프린트가 설정되어 있으면 생성
	{
		LobbyWidget = CreateWidget<UGameSessionWidget>(this, LobbyWidgetClass);
		if (LobbyWidget)
		{
			LobbyWidget->AddToViewport();
		}
	}
}

void ALobbyPlayerController::UpdateWidget()
{
	if (ALobbyGameState* GS = Cast<ALobbyGameState>( GetWorld()->GetGameState()))
	{
		if (LobbyWidget)
		{
			LobbyWidget->UpdatePlayerList(GS->PlayerList);  // UI 업데이트 함수 호출
		}
	}
}
