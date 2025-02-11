// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/ItemSpawn/DropSpawnVolume.h"

#include "Landscape.h"
#include "Building/SupplyDrop/SupplyDrop.h"

// Sets default values
ADropSpawnVolume::ADropSpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	SpawnArea = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	SpawnArea->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADropSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ADropSpawnVolume::SetDropLocation, 2.0f, true);
}

void ADropSpawnVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	//AActor* SpawnedZone = GetWorld()->SpawnActor<AActor>(zoneActorClass, GetRandomLandscapeLocation());
}

// FBox ADropSpawnVolume::GetLandscapeBounds()
// {
// 	// if (!TargetLandscape)
// 	// {
// 	// 	UE_LOG(LogTemp, Error, TEXT("Error: TargetLandscape is nullptr!"));
// 	// 	return FBox();
// 	// }
// 	//
// 	// //return TargetLandscape->GetComponentsBoundingBox();
// 	// FBox Bounds = TargetLandscape->GetComponentsBoundingBox();
// 	// return Bounds;
// }

FVector ADropSpawnVolume::GetRandomLandscapeLocation()
{
	// // ✅ 액터의 바운드 가져오기
	// FVector Origin, BoxExtent;
	// GetActorBounds(false, Origin, BoxExtent);
	//
	// // ✅ 액터 내부에서 랜덤한 X, Y, Z 좌표 생성
	// float RandX = FMath::RandRange(Origin.X - BoxExtent.X, Origin.X + BoxExtent.X);
	// float RandY = FMath::RandRange(Origin.Y - BoxExtent.Y, Origin.Y + BoxExtent.Y);
	// float RandZ = Origin.Z + 1000.0f;  // 필요하면 +100.0f 추가 가능
	//
	// UE_LOG(LogTemp, Warning, TEXT("Spawn Location Inside Actor Bounds: X=%f, Y=%f, Z=%f"), RandX, RandY, RandZ);
	//
	// return FVector(RandX, RandY, RandZ);
	
	if (!TargetLandscape)
	{
		UE_LOG(LogTemp, Error, TEXT("TargetLandscape is nullptr!"));
		return FVector::ZeroVector;
	}
	
	// ✅ 랜드스케이프 바운드 가져오기
	FBox Bounds = TargetLandscape->GetComponentsBoundingBox();
    
	// ✅ 랜드스케이프 내부에서 랜덤한 X, Y 좌표 생성
	float RandX = FMath::RandRange(Bounds.Min.X, Bounds.Max.X);
	float RandY = FMath::RandRange(Bounds.Min.Y, Bounds.Max.Y);
	float RandZ = Bounds.Max.Z + 1000.0f; 

	UE_LOG(LogTemp, Warning, TEXT("Spawn Location Inside Actor Bounds: X=%f, Y=%f, Z=%f"), RandX, RandY, RandZ);
	
	return FVector(RandX, RandY, RandZ);

	

	
}

// FVector ADropSpawnVolume::GetRandomLandscapeLocation()
// {
// 	if (!TargetLandscape)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("TargetLandscape is nullptr!"));
// 		return FVector::ZeroVector;
// 	}
//
// 	// ✅ 랜드스케이프 바운드 가져오기 (GetActorBounds로 수정)
// 	FVector Origin, BoxExtent;
// 	TargetLandscape->GetActorBounds(false, Origin, BoxExtent);
//
// 	if (BoxExtent.X == 0 || BoxExtent.Y == 0)  // 바운드 크기가 0이면 실패
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Invalid Landscape Bounds! Using default location."));
// 		return FVector::ZeroVector;
// 	}
//
// 	// ✅ 랜드스케이프 내부에서 랜덤한 X, Y 좌표 생성
// 	float RandX = FMath::RandRange(Origin.X - BoxExtent.X, Origin.X + BoxExtent.X);
// 	float RandY = FMath::RandRange(Origin.Y - BoxExtent.Y, Origin.Y + BoxExtent.Y);
// 	float RandZ = Origin.Z + 100.0f;  // 지면 위로 살짝 띄우기
//
// 	UE_LOG(LogTemp, Warning, TEXT("Spawn Location: X=%f, Y=%f, Z=%f"), RandX, RandY, RandZ);
//
// 	return FVector(RandX, RandY, RandZ);
// }

void ADropSpawnVolume::SetDropLocation()
{
	UWorld* World = GetWorld();
	
	ASupplyDrop* SupplyDrop = World->SpawnActor<ASupplyDrop>(BP_SupplyDrop,GetRandomLandscapeLocation(),FRotator::ZeroRotator);
}

// Called every frame
void ADropSpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

