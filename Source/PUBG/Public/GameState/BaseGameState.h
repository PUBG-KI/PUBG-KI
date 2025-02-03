// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseGameState.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ABaseGameState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 게임 시간
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
	int32 RemainingTime;

	// 플레이어 수
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
	int32 PlayerCount;
		
	// 게임 상태 업데이트
	void UpdateRemainingTime(int32 NewTime);
	
	// 플레이어 수 업데이트
	void UpdatePlayerCount();
};
