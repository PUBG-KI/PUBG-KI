// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "HudWidget.generated.h"

class UPlayerStatusWidget;
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

public:
	UFUNCTION()
	UPlayerStatusWidget* GetPlayerStatusWidget() const { return WBP_PlayerStatus; }
};
