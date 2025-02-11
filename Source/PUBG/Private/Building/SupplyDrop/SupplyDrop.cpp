// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/SupplyDrop/SupplyDrop.h"

#include "Landscape.h"

// Sets default values
ASupplyDrop::ASupplyDrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropMesh"));
	DropMesh->SetupAttachment(RootComponent);

	Balloon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Balloon"));
	Balloon->SetupAttachment(DropMesh);
	
}

// FBox ASupplyDrop::GetLandscapeBounds()
// {
// 	if (!TargetLandscape)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Error: TargetLandscape is nullptr!"));
// 		return FBox();
// 	}
//
// 	//return TargetLandscape->GetComponentsBoundingBox();
// 	FBox Bounds = TargetLandscape->GetComponentsBoundingBox();
// 	return Bounds;
// }
//
// //랜드스케이프 랜덤 위치 받아오기
// FVector ASupplyDrop::GetRandomLandscapeLocation()
// {
// 	// float WorldSize = 50000.0f;  // 맵 크기 조정
// 	// float RandX = FMath::RandRange(-WorldSize, WorldSize);
// 	// float RandY = FMath::RandRange(-WorldSize, WorldSize);
// 	// float RandZ = DropMesh->GetComponentLocation().Z + 1000.0f;  // 공중에서 시작
//
// 	//return FVector(RandX, RandY, RandZ);
// 	
// 	// FVector Origin, BoxExtent;
// 	// TargetLandscape->GetActorBounds(false, Origin, BoxExtent);
// 	//
// 	// float RandX = FMath::RandRange(Origin.X - BoxExtent.X, Origin.X + BoxExtent.X);
// 	// float RandY = FMath::RandRange(Origin.Y - BoxExtent.Y, Origin.Y + BoxExtent.Y);
// 	// float RandZ = Origin.Z + 1000.0f; // 기본적으로 100 높이 추가하여 지형 위로 띄우기
// 	//
// 	// UE_LOG(LogTemp, Warning, TEXT("Random Location: X=%f, Y=%f, Z=%f"), RandX, RandY, RandZ);
// 	//
// 	// return FVector(RandX, RandY, RandZ);
// 	
// 	// if (!TargetLandscape)
// 	// {
// 	// 	UE_LOG(LogTemp, Error, TEXT("Error: TargetLandscape is nullptr!"));
// 	// 	return FVector::ZeroVector;
// 	// }
// 	//
// 	// FVector Origin,BoxExtent;
// 	// TargetLandscape->GetActorBounds(false, Origin, BoxExtent);
// 	// //FBox Bounds = GetLandscapeBounds();
// 	//
// 	// //float LocationX = FMath::RandRange(Bounds.Min.X,Bounds.Max.X);
// 	// //float LocationY = FMath::RandRange(Bounds.Min.Y,Bounds.Max.Y);
// 	//
// 	// // UE_LOG(LogTemp, Display, TEXT("BoxExtent: %f"), BoxExtent.X);
// 	// // UE_LOG(LogTemp, Display, TEXT("Origin: %f"), Origin.X);
// 	// //
// 	// // UE_LOG(LogTemp, Display, TEXT("BoxExtent: %f"), BoxExtent.Y);
// 	// // UE_LOG(LogTemp, Display, TEXT("Origin: %f"), Origin.Y);
// 	//
// 	// UE_LOG(LogTemp, Warning, TEXT("TargetLandscape Name: %s"), *TargetLandscape->GetName());
// 	// UE_LOG(LogTemp, Warning, TEXT("Origin: X=%f, Y=%f, Z=%f"), Origin.X, Origin.Y, Origin.Z);
// 	// UE_LOG(LogTemp, Warning, TEXT("BoxExtent: X=%f, Y=%f, Z=%f"), BoxExtent.X, BoxExtent.Y, BoxExtent.Z);
// 	//
// 	//
// 	// float LocationX = FMath::RandRange(Origin.X - BoxExtent.X, Origin.X + BoxExtent.X);
// 	// float LocationY = FMath::RandRange(Origin.Y - BoxExtent.Y, Origin.Y + BoxExtent.Y);
// 	//
// 	// return FVector(LocationX,LocationY,DropMesh->GetComponentLocation().Z + 1000.0f);
// }
//
// void ASupplyDrop::SetDropLocation()
// {
// 	DropMesh->SetWorldLocation(GetRandomLandscapeLocation());
// }

// Called when the game starts or when spawned
void ASupplyDrop::BeginPlay()
{
	Super::BeginPlay();

	//SetDropLocation();
	
	//DropMesh->SetWorldLocation(FVector(DropMesh->GetComponentLocation().X, DropMesh->GetComponentLocation().Y, DropMesh->GetComponentLocation().Z + 1000.0f));
}

// void ASupplyDrop::OnConstruction(const FTransform& Transform)
// {
// 	Super::OnConstruction(Transform);
//
// 	
// 	
// }

// Called every frame
void ASupplyDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


