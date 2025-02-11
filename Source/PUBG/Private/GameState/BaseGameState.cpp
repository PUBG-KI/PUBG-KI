// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/BaseGameState.h"

#include "Controller/BasePlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

ABaseGameState::ABaseGameState()
{
	RemainingTime = 600; // 초기 게임 시간 설정 (10분)
	PlayerCount = 0;
	initalize();
}

void ABaseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Replicated 변수 등록
	DOREPLIFETIME(ABaseGameState, RemainingTime);
	DOREPLIFETIME(ABaseGameState, PlayerCount);
	DOREPLIFETIME(ABaseGameState, bIsGameStarted);
	DOREPLIFETIME(ABaseGameState, bIsPlaneBoarding);
	DOREPLIFETIME(ABaseGameState, CurrentZoneCenter);
	DOREPLIFETIME(ABaseGameState, CurrentZoneScale);
	DOREPLIFETIME(ABaseGameState, NextZoneCenter);
	DOREPLIFETIME(ABaseGameState, NextZoneScale);
	DOREPLIFETIME(ABaseGameState, bIsVisibiltyNextZone);
}

void ABaseGameState::SetGameStartNotification(bool bNewValue)
{
	if (bIsGameStarted != bNewValue)
	{
		bIsGameStarted = bNewValue;
		OnRep_GameStartNotification();//리슨서버용
	}	
}

void ABaseGameState::SetBoardPlaneNotification(bool bNewValue)
{
	if (bIsPlaneBoarding != bNewValue)
	{
		bIsPlaneBoarding = bNewValue;
		OnRep_BoardPlaneNotification();//리슨서버용
	}
}

void ABaseGameState::OnRep_PlayerCount()
{
	//UE_LOG(LogTemp, Warning, TEXT("OnRep_PlayerCount"));
	UpdateWidget();
}

void ABaseGameState::OnRep_RemainingTime()
{
	UE_LOG(LogTemp, Warning, TEXT("RemainingTime = %d"), RemainingTime);
}

void ABaseGameState::OnRep_GameStartNotification()
{
	UE_LOG(LogTemp, Log, TEXT("Game has started!"));
}

void ABaseGameState::OnRep_BoardPlaneNotification()
{
	UE_LOG(LogTemp, Log, TEXT("Board the plane!"));
}


void ABaseGameState::OnRep_CurrentZoneScale()
{	
	UE_LOG(LogTemp, Warning, TEXT("CurrentZoneCenter.X = %f, CurrentZoneCenter.Y = %f"), CurrentZoneCenter.X, CurrentZoneCenter.Y);	
	UE_LOG(LogTemp, Warning, TEXT("CurrentZoneScale = %f"), CurrentZoneScale);
}

void ABaseGameState::OnRep_IsVisibiltyNextZone()
{	
	UE_LOG(LogTemp, Warning, TEXT("NextZoneCenter.X = %f, NextZoneCenter.Y = %f"), NextZoneCenter.X, NextZoneCenter.Y);	
	UE_LOG(LogTemp, Warning, TEXT("NextZoneScale = %f"), NextZoneScale);
	if (bIsVisibiltyNextZone)
	{		
		UE_LOG(LogTemp, Warning, TEXT("bIsVisibiltyNextZone = true"));
	}
	else
	{		
		UE_LOG(LogTemp, Warning, TEXT("bIsVisibiltyNextZone = false"));
	}
}

void ABaseGameState::UpdateWidget()
{
	for (APlayerState* PS : PlayerArray)
	{
		if (PS && PS->GetOwner())  // PlayerState가 유효한 경우
		{
			ABasePlayerController* PC = Cast<ABasePlayerController>(PS->GetOwner());
			if (PC && PC->IsLocalController())  // 로컬 컨트롤러 확인
			{
				PC->UpdateCurrentPlayer(PlayerCount);
			}
		}
	}
}

void ABaseGameState::UpdateRemainingTime(int32 NewTime)
{
	RemainingTime = NewTime;
	OnRep_RemainingTime(); //리슨 서버용
}

void ABaseGameState::UpdateCurrentZone(FVector _CurrentZoneCenter, float _CurrentZoneScale)
{
	CurrentZoneCenter = _CurrentZoneCenter;
	CurrentZoneScale = _CurrentZoneScale;
}

void ABaseGameState::UpdateNextZone(FVector _NextZoneCenter, float _NextZoneScale, bool _bIsVisibiltyNextZone)
{
	NextZoneCenter = _NextZoneCenter;
	NextZoneScale = _NextZoneScale;
	bIsVisibiltyNextZone = _bIsVisibiltyNextZone;
}


void ABaseGameState::UpdatePlayerCount()
{	
	PlayerCount = PlayerArray.Num();
	OnRep_PlayerCount(); // 리슨 서버용
}

int32 ABaseGameState::GetPlayerCount()
{
	return PlayerCount;
}

bool ABaseGameState::GetIsGameStarted()
{
	return bIsGameStarted;
}

void ABaseGameState::initalize()
{
	// 기본 값 설정
	bIsGameStarted = false;
	bIsPlaneBoarding = false;
}
