// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class ACrosshairHUD;

/**
 * 
 */
UCLASS()
class PUBG_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

	
protected:
	ABasePlayerController();
	
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditAnywhere)
	ACrosshairHUD* HUD;

public:
	UFUNCTION(BlueprintCallable)
	void SetHUDTexture(float DeltaTime);

};


