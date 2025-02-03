// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BaseGameMode.h"
#include "GameState/BaseGameState.h"

ABaseGameMode::ABaseGameMode()
{
}

void ABaseGameMode::StartPlay()
{
	Super::StartPlay();
}

void ABaseGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UpdatePlayerCount();
}

void ABaseGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	UpdatePlayerCount();
}

void ABaseGameMode::UpdatePlayerCount() const
{
	ABaseGameState* BaseGameState = Cast<ABaseGameState>(GameState);
	BaseGameState->UpdatePlayerCount();
}
