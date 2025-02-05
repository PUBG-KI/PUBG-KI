// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"

class ALobbyPlayerState;
/**
 * 
 */
UCLASS()
class PUBG_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
		
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	
	
	UPROPERTY(ReplicatedUsing=OnRep_PlayerArray)
	TArray<ALobbyPlayerState*> PlayerList;  // 플레이어 목록

	void AddPlayerToList(ALobbyPlayerState* PlayerState);
	void RemovePlayerToList(ALobbyPlayerState* PlayerState);
	void SetIndexPlayerToList(ALobbyPlayerState* PlayerState, int Index);
	
	UFUNCTION()
	void OnRep_PlayerArray();  // UI 갱신

	void UpdateLobbyWidget();

	// 모든 플레이어가 준비되었는지 확인하는 함수
	void CheckAllPlayersReady();

	// 모든 플레이어가 준비 완료되었는지 여부 (Replicated)
	UPROPERTY(ReplicatedUsing=OnRep_AllReady)
	bool bAllPlayersReady;

	UFUNCTION()
	void OnRep_AllReady() const;  // 클라이언트 UI 업데이트

};
