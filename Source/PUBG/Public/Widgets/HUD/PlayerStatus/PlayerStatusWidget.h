// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "PlayerStatusWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class PUBG_API UPlayerStatusWidget : public UWidgetBase
{
	GENERATED_BODY()


private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UProgressBar* ProgressBar_Health;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UProgressBar* ProgressBar_FillingGaguge;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UProgressBar* ProgressBar_FillGaguge;

	UPROPERTY()
	float MaxHealth;
	UPROPERTY()
	float Health;
public:
	// Variable Setter
	UFUNCTION()
	void SetHealth(float OutHealth);
	void SetMaxHealth(float OutMaxHealth);
	
	//Widget Getter
	UFUNCTION()
	UProgressBar* GetProgressBar_Health() const { return ProgressBar_Health; }

	//Widget Setter
	UFUNCTION()
	void SetProgressBar_Health(float OutHealth);
};
