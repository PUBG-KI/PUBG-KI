// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/LobbyPlayerState.h"

#include "Controller/LobbyPlayerController.h"
#include "GameState/LobbyGameState.h"
#include "Net/UnrealNetwork.h"

ALobbyPlayerState::ALobbyPlayerState()
{
	PlayerName = GetPlayerName();
	bReplicates = true;
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ALobbyPlayerState, PlayerName);
    DOREPLIFETIME(ALobbyPlayerState, bIsReady);
    DOREPLIFETIME(ALobbyPlayerState, bIsHost);
}

bool ALobbyPlayerState::IsHost() const
{
	return bIsHost;
}

void ALobbyPlayerState::OnRep_Player_Name()
{
}

void ALobbyPlayerState::OnRep_IsReady()
{
	UE_LOG(LogTemp, Log, TEXT("Client : bIsReady : %s"), bIsReady ? TEXT("True") : TEXT("False"));
	
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwner());
	if (PC && PC->IsLocalController())  // 로컬 컨트롤러 확인
	{
		PC->UpdateWidget();
	}
}

void ALobbyPlayerState::OnRep_IsHost()
{
	UE_LOG(LogTemp, Log, TEXT("Client : bIsHost : %s"), bIsHost ? TEXT("True") : TEXT("False"));
}

void ALobbyPlayerState::ServerSetReady_Implementation(bool bNewReady)
{
	if (ALobbyGameState* GS = Cast<ALobbyGameState>( GetWorld()->GetGameState()))
	{
		
		//GS->PlayerList
		//TArray<ALobbyPlayerState*> Array = GS->PlayerList;
		//int Index = Array.Find((this));
		int Index = GS->PlayerList.Find((this));
		bIsReady = bNewReady;
		
		if (Index != -1)
		{
			GS->SetIndexPlayerToList(this, Index);
			//GS->PlayerList[Index] = this;
			//GS->PlayerList = Array;			
			//GS->OnRep_PlayerArray(GS->PlayerList);
		}
	}
}

bool ALobbyPlayerState::ServerSetReady_Validate(bool bNewReady)
{
	return true;
}
