// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "HudWidget.generated.h"

class UCurrentPlayerWidget;
class UPlayerStatusWidget;
class UWorldMapWidget;
/**
 * 
 */
UCLASS()
class PUBG_API UHudWidget : public UWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UPlayerStatusWidget* WBP_PlayerStatus;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UCurrentPlayerWidget* WBP_CurrentPlayer;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UWorldMapWidget* WBP_WorldMap;

public:
	UFUNCTION()
	UPlayerStatusWidget* GetPlayerStatusWidget() const { return WBP_PlayerStatus; }
	UFUNCTION()
	UCurrentPlayerWidget* GetCurrentPlayerWidget() const { return WBP_CurrentPlayer; }
	UFUNCTION()
	UWorldMapWidget* GetWorldMapWidget() const { return WBP_WorldMap; }
};
