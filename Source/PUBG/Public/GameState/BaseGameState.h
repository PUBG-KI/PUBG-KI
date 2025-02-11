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
	
	void SetGameStartNotification(bool bNewValue);
	void SetBoardPlaneNotification(bool bNewValue);

	UFUNCTION()
	void OnRep_PlayerCount();  // UI 갱신
	UFUNCTION()
	void OnRep_RemainingTime();  // UI 갱신
	// 게임 시작 알림을 위한 리플리케이션 함수
	UFUNCTION()
	void OnRep_GameStartNotification();
	// 비행기 탑승 알림을 위한 리플리케이션 함수
	UFUNCTION()
	void OnRep_BoardPlaneNotification();
	UFUNCTION()
	void OnRep_CurrentZoneScale();
	UFUNCTION()
	void OnRep_IsVisibiltyNextZone();
	
	void UpdateWidget();
	
	// 게임 상태 업데이트
	void UpdateRemainingTime(int32 NewTime);
	void UpdateCurrentZone(FVector CurrentZoneCenter, float CurrentZoneScale);
	void UpdateNextZone(FVector NextZoneCenter, float NextZoneScale, bool bIsVisibiltyNextZone);
	
	// 플레이어 수 업데이트
	void UpdatePlayerCount();
	int32 GetPlayerCount();
	bool GetIsGameStarted();

	void initalize();

private:
	// 게임 시간
	UPROPERTY(ReplicatedUsing=OnRep_RemainingTime, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	int32 RemainingTime;

	// 플레이어 수
	UPROPERTY(ReplicatedUsing=OnRep_PlayerCount, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	int32 PlayerCount;
		
	// 게임 시작 알림 (리플리케이션)
	UPROPERTY(ReplicatedUsing = OnRep_GameStartNotification, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	bool bIsGameStarted;

	// 비행기 탑승 알림 (리플리케이션)
	UPROPERTY(ReplicatedUsing = OnRep_BoardPlaneNotification, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	bool bIsPlaneBoarding;

	/** 현재 자기장의 중심 위치 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	FVector CurrentZoneCenter;

	/** 현재 자기장의 반지름 */
	UPROPERTY(ReplicatedUsing=OnRep_CurrentZoneScale, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	float CurrentZoneScale;

	/** 다음 자기장의 중심 위치 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	FVector NextZoneCenter;

	/** 다음 자기장의 반지름 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	float NextZoneScale;
	
	// 다음 원을 표시할 것인지
	UPROPERTY(ReplicatedUsing = OnRep_IsVisibiltyNextZone, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	bool bIsVisibiltyNextZone;
};


