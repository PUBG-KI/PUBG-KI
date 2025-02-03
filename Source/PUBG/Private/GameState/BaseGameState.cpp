// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/BaseGameState.h"
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
}

void ABaseGameState::UpdateRemainingTime(int32 NewTime)
{
	RemainingTime = NewTime;
}

void ABaseGameState::UpdatePlayerCount()
{	
	PlayerCount = UGameplayStatics::GetNumPlayerStates(GetWorld());
}
