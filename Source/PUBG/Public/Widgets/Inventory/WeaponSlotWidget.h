// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "WeaponSlotWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PUBG_API UWeaponSlotWidget : public UWidgetBase
{
	GENERATED_BODY()


private:
	// Widget Variable
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_RenderTargetWeapon;

public:
	// Getter
	UImage* GetImage_RenderTargetWeapon() const { return Image_RenderTargetWeapon; }
	
	//Setter
	void SetImage_RenderTargetWeapon(UImage* OutImage) { Image_RenderTargetWeapon = OutImage; }
};
