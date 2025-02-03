// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BasePlayerController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Components/WrapBox.h"
#include "HUD/Crosshair/CrosshairHUD.h"
#include "PlayerState/BasePlayerState.h"
#include "Widgets/HUD/HudWidget.h"
#include "Widgets/HUD/PlayerStatus/PlayerStatusWidget.h"
#include "Widgets/Inventory/InventoryWidget.h"

ABasePlayerController::ABasePlayerController()
{
	PrimaryActorTick.bCanEverTick = false;
	InventoryWidget = nullptr;
}

void ABasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void ABasePlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	if (!IsLocalPlayerController())
	{
		return;
	}

	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	if (!PS)
	{
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	
	//
	// if (IsValid(InventoryWidgetClass))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Inventory Widget Loaded"));
	// 	
	// 	InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
	// 	InventoryWidget->AddToViewport();
	// 	if (PlayerCharacter)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("GetOwningPlayer"));
	// 		InventoryWidget->SetInventoryComponent(PlayerCharacter->GetInventoryComponent());
	//
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("FailGetOwningPlayer"));
	// 	}
	// 	InventoryWidget->GetWrapBox_Inventory()->ClearChildren();
	// 	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	// }
		

	if (IsValid(HudWidgetClass))
	{
		HudWidget = CreateWidget<UHudWidget>(this, HudWidgetClass);
		HudWidget->AddToViewport();
	}

	HudWidget->GetPlayerStatusWidget()->SetHealth(PS->GetHealth());
	HudWidget->GetPlayerStatusWidget()->SetMaxHealth(PS->GetMaxHealth());
	HudWidget->GetPlayerStatusWidget()->SetPlayerCharacter(PlayerCharacter);

	InputModeGame();	
}

void ABasePlayerController::InputModeUI()
{
	SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, InventoryWidget, EMouseLockMode::DoNotLock, true);
}

void ABasePlayerController::InputModeGame()
{
	SetShowMouseCursor(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
}


void ABasePlayerController::CreateInventoryWidget()
{

	if (InventoryWidget != nullptr)
	{
		InventoryWidget->RemoveFromParent();
	}
	
	if (IsValid(InventoryWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Widget Loaded"));
		
		InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
		
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
		UE_LOG(LogTemp, Warning, TEXT("GetOwningPlayer"));
		InventoryWidget->SetInventoryComponent(PlayerCharacter->GetInventoryComponent());
		InventoryWidget->SetNearComponent(PlayerCharacter->GetNearComponent());

		InventoryWidget->GetWrapBox_Inventory()->ClearChildren();
		InventoryWidget->GetWrapBox_Near()->ClearChildren();
		InventoryWidget->AddToViewport();
	}

	// APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	//
	// if (PlayerCharacter)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("GetOwningPlayer"));
	// 	InventoryWidget->SetInventoryComponent(PlayerCharacter->GetInventoryComponent());
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("FailGetOwningPlayer"));
	// }
	
	// InventoryWidget->GetWrapBox_Inventory()->ClearChildren();
	// InventoryWidget->AddToViewport();
}

void ABasePlayerController::DestroyInventoryWidget()
{
	if (InventoryWidget)
	{
		InventoryWidget->RemoveFromParent();
		InventoryWidget = nullptr;
	}
}
	


