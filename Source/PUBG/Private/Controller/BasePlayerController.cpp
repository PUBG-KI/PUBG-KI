// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BasePlayerController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Components/WrapBox.h"
#include "HUD/Crosshair/CrosshairHUD.h"
#include "Widgets/Inventory/InventoryWidget.h"

ABasePlayerController::ABasePlayerController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void ABasePlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	if (IsValid(InventoryWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Widget Loaded"));
		
		InventoryWidget = Cast<UInventoryWidget>(CreateWidget(this, InventoryWidgetClass));
		if (InventoryWidget != nullptr)
		{
			InventoryWidget->AddToViewport();
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
			if (PlayerCharacter)
			{
				UE_LOG(LogTemp, Warning, TEXT("GetOwningPlayer"));
				InventoryWidget->SetInventoryComponent(PlayerCharacter->GetInventoryComponent());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("FailGetOwningPlayer"));
			}
			InventoryWidget->GetWrapBox_Inventory()->ClearChildren();
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Widget not found"));
	}
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

