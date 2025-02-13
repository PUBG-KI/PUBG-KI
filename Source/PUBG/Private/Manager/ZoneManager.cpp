// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ZoneManager.h"

#include "GameInstance/BaseGameInstance.h"
#include "GameplayActor/Zone/Zone.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/LandscapeManager.h"

UZoneManager::UZoneManager()
{
}

void UZoneManager::InitializeManager()
{
	if (SpawnedZone)
	{
		SpawnedZone->Destroy();
	}
	
	SpawnedZone = nullptr;
}

void UZoneManager::SpawnZone()
{
	UBaseGameInstance* GI = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GI)
	{		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(FVector(0, 0, 0));
		SpawnTransform.SetRotation(FQuat::Identity);	
		SpawnTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		//액터 스폰
		SpawnedZone = GetWorld()->SpawnActor<AZone>(ZoneClass, SpawnTransform);
		
		FBox ZoneBox = SpawnedZone->GetComponentsBoundingBox();		
		FBox LandscapeBox = GI->GetLandscapeManager()->GetLandscapeBoundingBox();

		SpawnedZone->SetActorScale3D(FVector(LandscapeBox.Max.X / ZoneBox.Max.X, LandscapeBox.Max.Y / ZoneBox.Max.Y, LandscapeBox.Max.Z / ZoneBox.Max.Z * 2 + 10000.f) );

		UE_LOG(LogTemp, Warning, TEXT("SpawnedZone->GetActorScale3D() : %s"), *SpawnedZone->GetActorScale3D().ToString());

		SpawnedZone->StartShrinkTimer();
	}
}
