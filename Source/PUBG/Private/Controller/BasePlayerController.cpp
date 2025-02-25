// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BasePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Components/WrapBox.h"
#include "GameplayActor/Airplane/Airplane.h"
#include "GameState/BaseGameState.h"
#include "HUD/Crosshair/CrosshairHUD.h"
#include "PlayerState/BasePlayerState.h"
#include "Widgets/HUD/HudWidget.h"
#include "Widgets/HUD/PlayerStatus/PlayerStatusWidget.h"
#include "Widgets/HUD/GameStatus/CurrentPlayerWidget.h"
#include "Widgets/Inventory/InventoryWidget.h"
#include "Widgets/Map/MapWidget.h"
#include "Widgets/Map/WorldMapWidget.h"

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
		UE_LOG(LogTemp, Warning, TEXT("!IsLocalPlayerController"));
		return;
	}

	
	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	if (!PS)
	{		
		UE_LOG(LogTemp, Warning, TEXT("!ABasePlayerState"));
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
		
		UE_LOG(LogTemp, Warning, TEXT("HudWidgetClass Loaded"));
		HudWidget = CreateWidget<UHudWidget>(this, HudWidgetClass);
		HudWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HudWidgetClass is NULL"));
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


void ABasePlayerController::HitEventServer_Implementation(AActor* TargetActor, FGameplayTag HitTag,
                                                          FGameplayEventData payload)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, HitTag, payload);
}

bool ABasePlayerController::HitEventServer_Validate(AActor* TargetActor, FGameplayTag HitTag,
	FGameplayEventData payload)
{
	return true;
}


void ABasePlayerController::CreateInventoryWidget()
{
	if (!IsLocalController())  // 로컬 플레이어인지 확인
	{
		UE_LOG(LogTemp, Error, TEXT("CreateInventoryWidget() - Only Local Player Controllers can create widgets!"));
		return;
	}

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
		InventoryWidget->SetEquippedComponent(PlayerCharacter->GetEquippedComponent());


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

void ABasePlayerController::CreateWorldMapWidget()
{
	if (!IsLocalController())  // 로컬 플레이어인지 확인
	{
		UE_LOG(LogTemp, Error, TEXT("CreateInventoryWidget() - Only Local Player Controllers can create widgets!"));
		return;
	}

	if (WorldMapWidget)
	{
		WorldMapWidget->RemoveFromParent();
		WorldMapWidget = nullptr;
	}
	else if (IsValid(WorldMapWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("WorldMapWidget Loaded"));
		
		WorldMapWidget = CreateWidget<UWorldMapWidget>(this, WorldMapWidgetClass);	
		WorldMapWidget->AddToViewport();
	}
}

void ABasePlayerController::ClientCreateInventoryWidget_Implementation()
{
	if (!IsLocalController())  // 로컬 플레이어인지 확인
	{
		UE_LOG(LogTemp, Error, TEXT("CreateInventoryWidget() - Only Local Player Controllers can create widgets!"));
		return;
	}

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
		InventoryWidget->SetEquippedComponent(PlayerCharacter->GetEquippedComponent());

		InventoryWidget->GetWrapBox_Inventory()->ClearChildren();
		InventoryWidget->GetWrapBox_Near()->ClearChildren();
		InventoryWidget->AddToViewport();
	}
}

void ABasePlayerController::DestroyInventoryWidget()
{
	if (InventoryWidget)
	{
		InventoryWidget->RemoveFromParent();
		InventoryWidget = nullptr;
	}
}

void ABasePlayerController::UpdateCurrentPlayer(int32 CurrentPlayer)
{
	if (HudWidget)
	{		
		UCurrentPlayerWidget* CurrentPlayerWidget = HudWidget->GetCurrentPlayerWidget();
		if (CurrentPlayerWidget)
		{
			CurrentPlayerWidget->UpdateCurrentPlayer(CurrentPlayer);
		}
	}
}

void ABasePlayerController::UpdateMapCurrentZone()
{
	if (WorldMapWidget)
	{
		WorldMapWidget->GetWBP_Map()->UpdateCurrentZone();
	}
}

void ABasePlayerController::UpdateMapNextZone()
{
	if (WorldMapWidget)
	{
		WorldMapWidget->GetWBP_Map()->UpdateNextZone();
	}
}

void ABasePlayerController::Client_RemoveMappingContext_Implementation()
{
	// Enhanced Input Subsystem 가져오기
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		
	if (InputSubsystem && AdditionalInputMappingContext)
	{
		InputSubsystem->RemoveMappingContext(AdditionalInputMappingContext);	
	}
}

void ABasePlayerController::Client_AddMappingContext_Implementation(AAirplane* NewControlledAirplane, 
                                                                    UInputMappingContext* InputMappingContext)
{
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Error, TEXT("!IsLocalController()"));		
		return;  
	}
	
	if (!InputMappingContext)
	{
		UE_LOG(LogTemp, Error, TEXT("!InputMappingContext"));		
		return; 
	}	
	// Enhanced Input Subsystem 가져오기
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	if (InputSubsystem && InputMappingContext)
	{
		//ControlledAirplane = NewControlledAirplane;
		AdditionalInputMappingContext = InputMappingContext;
		InputSubsystem->AddMappingContext(InputMappingContext, 1);			
	}
}


	


