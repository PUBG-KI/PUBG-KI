// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimeManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PUBG_API UTimeManager : public UObject
{
	GENERATED_BODY()
	

public:
	UTimeManager();

	/** 매니저 초기화 */
	void InitializeManager();

	/** 게임 시작 타이머 */
	void StartGameTimer();
	
	/** 카운트다운 시작 타이머 */
	void CountDownGameTimer();

	/** 게임 시작 알림을 클라이언트에게 보내는 함수 */
	void NotifyClientsToStartGame();

	/** 비행기 타기 시작 알림 */
	void NotifyClientsToBoardPlane();

private:
	/** 게임 시작 타이머 */
	FTimerHandle GameTimerHandle;

	/** 게임 타이머 시간 */
	int32 CurrentGameTime;
	int32 StartGameTime;
	float TickTime;

	/** 클라이언트에 게임 시작 알림 */
	void GameStartCountdown();

	/** 비행기 태우기 시작 */
	void StartPlaneDeparture();
};
