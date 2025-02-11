// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/BaseGameInstance.h"

#include "Manager/GameEventManager.h"
#include "Manager/TimeManager.h"
#include "Manager/ZoneManager.h"

UBaseGameInstance::UBaseGameInstance()
{
	TimeManager = nullptr;
}

void UBaseGameInstance::Init()
{
	Super::Init();
	
	// 시간 관리 매니저 초기화
	TimeManager = NewObject<UTimeManager>(this, UTimeManager::StaticClass());
	TimeManager->InitializeManager();
	GameEventManager = NewObject<UGameEventManager>(this, UGameEventManager::StaticClass());
	GameEventManager->InitializeManager();
	ZoneManager = NewObject<UZoneManager>(this, ZoneManagerClass);
	ZoneManager->InitializeManager();
}
