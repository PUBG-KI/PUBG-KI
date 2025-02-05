// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/ItemSpawn/ItemSpawnerComponent.h"

#include "Misc/MapErrors.h"

// Sets default values for this component's properties
UItemSpawnerComponent::UItemSpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnItems();
	// ...
	
}


// Called every frame
void UItemSpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UItemSpawnerComponent::SpawnItems()
{
	UWorld* World = GetWorld();

	for (const FVector& SpawnLocation : SpawnLocations)
	{
		for (int32 i = 0; i < SpawnItemCount; i++)
		{
			//위치 주변 배치
			FVector RandomOffset = FVector(
				FMath::RandRange(-ItemSpawnRadius, ItemSpawnRadius),
				0.0f,
				0.0f
			);
			FVector FinalLocation = SpawnLocation + RandomOffset;
			
			//아이템 랜덤 생성
			World->SpawnActor<AActor>(BP_Item,FinalLocation,FRotator::ZeroRotator);	
		}
		
	}
	
}

