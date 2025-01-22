// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BasePlayerController.h"

#include "HUD/Crosshair/CrosshairHUD.h"

ABasePlayerController::ABasePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	HUD = nullptr;
}

void ABasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetHUDTexture(DeltaTime);
}

void ABasePlayerController::SetHUDTexture(float DeltaTime)
{
	FHUDTexture HUDTexture;

	HUD = Cast<ACrosshairHUD>(GetHUD());
	if (HUD)
	{
		HUDTexture.Player_Crosshair_Center = 
	}
}
