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
}

void ABaseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Replicated 변수 등록
	DOREPLIFETIME(ABaseGameState, RemainingTime);
	DOREPLIFETIME(ABaseGameState, PlayerCount);
	DOREPLIFETIME(ABaseGameState, bIsPlayStart);
}

void ABaseGameState::OnRep_PlayerCount()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_PlayerCount"));
	UpdateWidget();
}

void ABaseGameState::OnRep_IsPlayStart()
{
}

void ABaseGameState::UpdateWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerCount = %d"), PlayerCount);
	for (APlayerState* PS : PlayerArray)
	{
		if (PS && PS->GetOwner())  // PlayerState가 유효한 경우
		{
			UE_LOG(LogTemp, Warning, TEXT("PC Check"));
			ABasePlayerController* PC = Cast<ABasePlayerController>(PS->GetOwner());
			if (PC && PC->IsLocalController())  // 로컬 컨트롤러 확인
			{
				UE_LOG(LogTemp, Warning, TEXT("UpdateCurrentPlayer"));
				PC->UpdateCurrentPlayer(PlayerCount);
			}
		}
	}
}

void ABaseGameState::UpdateRemainingTime(int32 NewTime)
{
	RemainingTime = NewTime;
}

void ABaseGameState::UpdatePlayerCount()
{	
	PlayerCount = PlayerArray.Num();
}

int32 ABaseGameState::GetPlayerCount()
{
	return PlayerCount;
}
