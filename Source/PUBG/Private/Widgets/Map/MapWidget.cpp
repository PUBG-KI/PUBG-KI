// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Map/MapWidget.h"

#include "Components/Image.h"
#include "GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"

UMapWidget::UMapWidget()
{
}

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	UpdatePlayerLocation();
}

void UMapWidget::UpdatePlayerLocation()
{
	FVector2D NewLocation = ConvertWorldToMap(GetOwningPlayerPawn()->GetActorLocation());

	FWidgetTransform NewTransform;
	NewTransform.Translation = NewLocation;
	Image_Player->SetRenderTransform(NewTransform);
}

FVector2D UMapWidget::ConvertWorldToMap(FVector WorldLocation) const
{
	ABaseGameState* GS = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (!GS)
	{
		return FVector2D::ZeroVector;		
	}

	FBox LandScapeBoundingBox = GS->GetLandScapeBoundingBox();
	
	float MaxX = LandScapeBoundingBox.Max.X;
	float MaxY = LandScapeBoundingBox.Max.Y;
	float MinX = LandScapeBoundingBox.Min.X;
	float MinY = LandScapeBoundingBox.Min.Y;
	
	float MapWidth = Image_WorldMap->GetBrush().ImageSize.X;
	float MapHeight = Image_WorldMap->GetBrush().ImageSize.Y;
	
	float UI_X = (WorldLocation.X - MinX) / (MaxX - MinX) * MapWidth;
	float UI_Y = (WorldLocation.Y - MinY) / (MaxY - MinY) * MapHeight;

	return FVector2D(UI_X, UI_Y);
}
