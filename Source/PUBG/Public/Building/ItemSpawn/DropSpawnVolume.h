// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "DropSpawnVolume.generated.h"

class ALandscape;
class ASupplyDrop;

UCLASS()
class PUBG_API ADropSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropSpawnVolume();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<ALandscape> TargetLandscape;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<ASupplyDrop> BP_SupplyDrop;

private:
	//자기장이 생성될 영역
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* SpawnArea;

	FTimerHandle SpawnTimerHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
	FVector SpawnScale;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	//랜드스케이프 크기 가져오기
	FBox GetLandscapeBounds();
	
	//랜드스케이프 랜덤 위치 지정
	FVector GetRandomLandscapeLocation();

	//드롭 위치 세팅
	void SetDropLocation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
