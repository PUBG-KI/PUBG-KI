// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ZoneManager.h"

#include "Building/Zone/Zone.h"

UZoneManager::UZoneManager()
{
}

void UZoneManager::InitializeManager()
{
	if (SpawnedZone)
	{
		SpawnedZone->Destroy();
	}
}

void UZoneManager::SpawnZone()
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(FVector(0, 0, 0));
	SpawnTransform.SetRotation(FQuat::Identity);
	SpawnTransform.SetScale3D(FVector(50.0f, 50.0f, 50.0f));

	//액터 스폰
	SpawnedZone = GetWorld()->SpawnActor<AZone>(ZoneClass, SpawnTransform);
	SpawnedZone->StartShrinkZone();
}
