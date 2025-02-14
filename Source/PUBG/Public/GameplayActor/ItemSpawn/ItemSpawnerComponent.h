// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSpawnerComponent.generated.h"

class AItemBase;
struct FItemStruct;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUBG_API UItemSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemSpawnerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	//TestItem 스폰시킬 함수
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnItems();

	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SetRandomMesh(AItemBase* Item, FName ItemRowName);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FName GetRandomItemRowName();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SetRandomProperties(AItemBase* Item, FName ItemRowName);
	

private:
	//스폰존 생성 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TArray<FVector> SpawnLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BP_Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	int32 SpawnItemCount;

	//아이템 스폰 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	int32 ItemSpawnRadius;

	//아이템 데이터테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	UDataTable* SpawnItemTable;
	
};
