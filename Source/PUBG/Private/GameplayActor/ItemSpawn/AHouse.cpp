// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/ItemSpawn/AHouse.h"

// Sets default values
AAHouse::AAHouse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//집 메쉬
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	HouseMesh = CreateDefaultSubobject<UStaticMeshComponent>("HouseMesh");
	HouseMesh->SetupAttachment(RootComponent);
	
	//스폰컴포넌트
	ItemSpawnerComponent = CreateDefaultSubobject<UItemSpawnerComponent>(TEXT("ItemSpawnerComponent"));
	
}

// Called when the game starts or when spawned
void AAHouse::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAHouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

