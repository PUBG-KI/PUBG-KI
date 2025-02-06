// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "LobbyPlayerController.generated.h"

class UGameSessionWidget;
class ALobbyPlayerState;
/**
 * 
 */
UCLASS()
class PUBG_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	void ShowLobby();
	void UpdateWidget();
	void MoveToDedicatedServer(const FString& ConnectIP, const FString& ConnectMap);
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UGameSessionWidget* LobbyWidget;	

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameSessionWidget> LobbyWidgetClass;	

};
