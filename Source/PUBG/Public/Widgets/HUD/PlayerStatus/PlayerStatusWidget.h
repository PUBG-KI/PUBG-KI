// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "PlayerStatusWidget.generated.h"

class APlayerCharacter;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PUBG_API UPlayerStatusWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

private:
	// Widget Variable
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UProgressBar* ProgressBar_Health;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UProgressBar* ProgressBar_FillingGaguge;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UProgressBar* ProgressBar_FillGaguge;

	// Variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	APlayerCharacter* PlayerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	float Health;
public:
	// Variable Setter
	UFUNCTION(BlueprintCallable)
	void SetPlayerCharacter(APlayerCharacter* OutPlayerCharacter) { PlayerCharacter = OutPlayerCharacter; }
	UFUNCTION(BlueprintCallable)
	void SetHealth(float OutHealth);
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float OutMaxHealth);
	
	//Widget Getter
	UFUNCTION()
	UProgressBar* GetProgressBar_Health() const { return ProgressBar_Health; }

	//Widget Setter
	UFUNCTION(BlueprintCallable)
	void SetProgressBar_Health(float OutHealth);
};
