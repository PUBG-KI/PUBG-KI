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

	FInputModeUIOnly InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//InputModeData.SetWidgetToFocus(LobbyWidget->TakeWidget());

	SetInputMode(InputModeData);
	bShowMouseCursor = true;
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

void ALobbyPlayerController::MoveToDedicatedServer(const FString& ConnectIP, const FString& ConnectMap)
{
	if (HasAuthority())
	{
		FString DedicatedServerAddress = ConnectIP; // 데디케이티드 서버 IP
		FString MapName = ConnectMap; // 데디케이티드 서버에서 실행 중인 맵 이름
		FString TravelURL = FString::Printf(TEXT("%s%s"), *DedicatedServerAddress, *MapName);

		ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
		//GetWorld()->ServerTravel(TravelURL, true);
	}
}
