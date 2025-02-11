// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameEventManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PUBG_API UGameEventManager : public UObject
{
	GENERATED_BODY()

public:
	void InitializeManager();
};
