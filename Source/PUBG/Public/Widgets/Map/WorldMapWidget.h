// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "WorldMapWidget.generated.h"

class UMapWidget;

/**
 * 
 */
UCLASS()
class PUBG_API UWorldMapWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	UMapWidget* GetWBP_Map();
	
private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UMapWidget* WBP_Map;
};
