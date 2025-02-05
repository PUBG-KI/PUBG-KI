// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"

#include "GameState/LobbyGameState.h"
#include "PlayerState/LobbyPlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
		
	if (ALobbyGameState* GS = GetGameState<ALobbyGameState>())
	{
		if (ALobbyPlayerState* PS = NewPlayer->GetPlayerState<ALobbyPlayerState>())
		{
			if (NewPlayer->HasAuthority() && NewPlayer->IsLocalController())
			{
				PS->bIsHost = true;
			}
			else
			{
				PS->bIsHost = false;
			}
			GS->AddPlayerToList(PS);
			//GS->PlayerList.Add(PS);
			//ForceNetUpdate();
			//GS->OnRep_PlayerArray();  // 클라이언트 UI 갱신
		}
	}
}


void ALobbyGameMode::Logout(AController* ExitingPlayer)
{
	Super::Logout(ExitingPlayer);
	if (ALobbyGameState* GS = GetGameState<ALobbyGameState>())
	{
		if (ALobbyPlayerState* PS = ExitingPlayer->GetPlayerState<ALobbyPlayerState>())
		{
			GS->RemovePlayerToList(PS);
			//GS->PlayerList.Remove(PS);
			//ForceNetUpdate();
			//GS->OnRep_PlayerArray();  // 클라이언트 UI 갱신
		}
	}
}
