// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UHudWidget;
class UInventoryWidget;
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
	virtual void BeginPlayingState() override;
	
private:
	

public:
	UFUNCTION(BlueprintCallable)
	void InputModeUI();
	UFUNCTION(BlueprintCallable)
	void InputModeGame();

	UFUNCTION(Server, Reliable, WithValidation)
	void HitEventServer(AActor* TargetActor, FGameplayTag HitTag, FGameplayEventData payload);
	
	//Getter
	UFUNCTION(BlueprintCallable)
	UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

	UFUNCTION(BlueprintCallable)
	void CreateInventoryWidget();
	UFUNCTION(Client, Reliable)
	void ClientCreateInventoryWidget();
	UFUNCTION(BlueprintCallable)
	void DestroyInventoryWidget();
	UFUNCTION(BlueprintCallable)
	void UpdateCurrentPlayer(int32 CurrentPlayer);
private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UHudWidget> HudWidgetClass;
	UPROPERTY()
	UHudWidget* HudWidget;
	
};


