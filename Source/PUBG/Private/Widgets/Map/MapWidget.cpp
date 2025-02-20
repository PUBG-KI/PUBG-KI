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

	SetLandScapeBoundingBox();
	
	if (Image_CurrentZone && Image_CurrentZone->GetDynamicMaterial())
	{
		// 기존 머터리얼을 동적 인스턴스로 변경
		CurrentZoneMaterial = Image_CurrentZone->GetDynamicMaterial();
	}
	if (Image_NextZone && Image_NextZone->GetDynamicMaterial())
	{
		// 기존 머터리얼을 동적 인스턴스로 변경
		NextZoneMaterial = Image_NextZone->GetDynamicMaterial();
	}

	UpdatePlayerLocation();
	UpdateCurrentZone();
	UpdateNextZone();
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

void UMapWidget::UpdateCurrentZone()
{
	ABaseGameState* GS = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (!GS)
	{
		return;		
	}
	FVector Center = GS->CurrentZoneCenter;
	float Scale = GS->CurrentZoneScale;
		
	FVector2D NewLocation = ConvertPosition(Center);
	
	Scale = ((Scale * 100) * 0.5f / LandScapeBoundingBox.Max.X);
	
	UE_LOG(LogTemp, Warning, TEXT("NewLocation : %s"), *NewLocation.ToString());	
	UE_LOG(LogTemp, Warning, TEXT("Radius = %f"), Scale);


	if (CurrentZoneMaterial)
	{
		CurrentZoneMaterial->SetVectorParameterValue(FName("Center"), FVector(NewLocation.X, NewLocation.Y, 0));
		CurrentZoneMaterial->SetScalarParameterValue(FName("Radius"), Scale);
	}
}

void UMapWidget::UpdateNextZone()
{
	ABaseGameState* GS = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (!GS)
	{
		return;		
	}
	
	bool IsVisibiltyNextZone = GS->bIsVisibiltyNextZone;

	if (!IsVisibiltyNextZone)
	{
		Image_NextZone->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	Image_NextZone->SetVisibility(ESlateVisibility::Visible);
	
	FVector Center = GS->NextZoneCenter;
	float Scale = GS->NextZoneScale;
	
	FVector2D NewLocation = ConvertPosition(Center);
	
	Scale = ((Scale * 100) * 0.5f / LandScapeBoundingBox.Max.X);
	
	UE_LOG(LogTemp, Warning, TEXT("NewLocation : %s"), *NewLocation.ToString());	
	UE_LOG(LogTemp, Warning, TEXT("Radius = %f"), Scale);

	if (NextZoneMaterial)
	{
		NextZoneMaterial->SetVectorParameterValue(FName("Center"), FVector(NewLocation.X, NewLocation.Y, 0));
		NextZoneMaterial->SetScalarParameterValue(FName("Radius"), Scale);
	}
}

void UMapWidget::SetLandScapeBoundingBox() 
{
	ABaseGameState* GS = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (!GS)
	{
		return;
	}

	LandScapeBoundingBox = GS->GetLandScapeBoundingBox();
}

FVector2D UMapWidget::ConvertPosition(const FVector& WorldLocation) const
{
	float MaxX = LandScapeBoundingBox.Max.X;
	float MaxY = LandScapeBoundingBox.Max.Y;
	float MinX = LandScapeBoundingBox.Min.X;
	float MinY = LandScapeBoundingBox.Min.Y;	

	float X = (WorldLocation.X - MinX) / (MaxX - MinX);
	float Y = (WorldLocation.Y - MinY) / (MaxY - MinY);
	
	return FVector2D(X, Y);
}

FVector2D UMapWidget::ConvertWorldToMap(const FVector& WorldLocation) const
{
	FVector2D Position = ConvertPosition(WorldLocation);
	
	float MapWidth = Image_WorldMap->GetBrush().ImageSize.X;
	float MapHeight = Image_WorldMap->GetBrush().ImageSize.Y;
	
	float UI_X = Position.X * MapWidth;
	float UI_Y = Position.Y * MapHeight;
	
	return FVector2D(UI_X, UI_Y);
}
