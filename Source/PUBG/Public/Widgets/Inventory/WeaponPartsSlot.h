// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "WeaponPartsSlot.generated.h"

class UButton;
class UImage;
/**
 * 
 */
UCLASS()
class PUBG_API UWeaponPartsSlot : public UWidgetBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UButton* Button_Parts;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Hovered;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Pressed;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Disabled;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Normal;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Parts;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Equipable;

public:
	// Getter Functions
	UFUNCTION(BlueprintCallable, Category = "UI")
	UButton* GetButtonParts() const { return Button_Parts; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageHovered() const { return Image_Hovered; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImagePressed() const { return Image_Pressed; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageDisabled() const { return Image_Disabled; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageNormal() const { return Image_Normal; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageParts() const { return Image_Parts; }

	// Setter Functions
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetButtonParts(UButton* NewButton) { Button_Parts = NewButton; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageHovered(UImage* NewImage) { Image_Hovered = NewImage; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImagePressed(UImage* NewImage) { Image_Pressed = NewImage; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageDisabled(UImage* NewImage) { Image_Disabled = NewImage; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageNormal(UImage* NewImage) { Image_Normal = NewImage; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageParts(UImage* NewImage) { Image_Parts = NewImage; }
	
};
