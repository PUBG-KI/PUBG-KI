// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSpawnerComponent.generated.h"

class AItemBase;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<FVector> SpawnLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AActor> BP_Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 SpawnItemCount;

	//아이템 스폰 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 ItemSpawnRadius;

	//아이템 데이터테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	UDataTable* SpawnItemTable;
	
	//스폰시킬 함수
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnItems();

private:
	//bool GetRandomItemRowName();
};
