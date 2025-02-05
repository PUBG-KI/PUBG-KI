// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSpawnerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUBG_API UItemSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemSpawnerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//스폰존 생성 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnLocation")
	TArray<FVector> SpawnLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnItem")
	TSubclassOf<AActor> BP_Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnItemCount")
	int32 SpawnItemCount;

	//아이템 스폰 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnItemRadius")
	int32 ItemSpawnRadius;
	
	//스폰시킬 함수
	UFUNCTION(BlueprintCallable, Category = "SpawnItems")
	void SpawnItems();
		
};
