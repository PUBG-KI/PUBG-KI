// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"

class UTimeManager;
class UGameEventManager;
class UZoneManager;

/**
 * 
 */
UCLASS()
class PUBG_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UBaseGameInstance();
	
	virtual void Init() override;

	/** 시간 관리 매니저 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UTimeManager* TimeManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UGameEventManager* GameEventManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UZoneManager* ZoneManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSubclassOf<UZoneManager> ZoneManagerClass;

	UTimeManager* GetTimeManager() const { return TimeManager; }
	UGameEventManager* GetGameEventManager() const { return GameEventManager; }
	UZoneManager* GetZoneManager() const { return ZoneManager; }
};
