// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "MapWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PUBG_API UMapWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	UMapWidget();

	virtual void NativeConstruct() override;	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	FVector2D ConvertWorldToMap(const FVector& WorldLocation) const;

	void UpdatePlayerLocation();
	void UpdateCurrentZone();
	void UpdateNextZone();
	void SetLandScapeBoundingBox();
	FVector2D ConvertPosition(const FVector& WorldLocation) const;

private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_WorldMap;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_Player;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_CurrentZone;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_NextZone;
	UPROPERTY()
	UMaterialInstanceDynamic* CurrentZoneMaterial;
	UMaterialInstanceDynamic* NextZoneMaterial;
	
	FBox LandScapeBoundingBox;
};
