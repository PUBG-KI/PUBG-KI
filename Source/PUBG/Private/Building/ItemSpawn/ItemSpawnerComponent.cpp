// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/ItemSpawn/ItemSpawnerComponent.h"

#include "Component/ItemData/ItemDataComponent.h"
#include "Item/ItemBase.h"

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

	AActor* Owner = GetOwner();

	for (const FVector& SpawnLocation : SpawnLocations)
	{
		for (int32 i = 0; i < SpawnItemCount; i++)
		{
			FVector WorldSpawnLocation = Owner->GetTransform().TransformPosition(SpawnLocation);

			//위치 주변 배치
			FVector RandomOffset = FVector(
				FMath::RandRange(-ItemSpawnRadius, ItemSpawnRadius),
				FMath::RandRange(-ItemSpawnRadius, ItemSpawnRadius),
				0.0f
			);
			
			FVector FinalLocation = WorldSpawnLocation + RandomOffset;
			
			//아이템 랜덤 생성
			AItemBase* SpawnedItem = World->SpawnActor<AItemBase>(BP_Item,FinalLocation,FRotator::ZeroRotator);

			if (SpawnedItem)
			{
				//SpawnedItem->SetItem();
				// UItemDataComponent* ItemDataComponent = SpawnedItem->GetItemDataComponent();
				// TArray<FName> RowNames = ItemDataComponent->;
			}
		}
		
	}
	
}

// FName UItemSpawnerComponent::GetRandomItemRowName()
// {
// 	TArray<FName> RowNames = SpawnItemTable->GetRowNames();
// 	FName RandomRowName = RowNames[FMath::RandRange(0,RowNames.Num()-1)];
// 	FItemStruct* FoundItem = SpawnItemTable->FindRow<FItemStruct>(RandomRowName);
// 	return RandomRowName;
//
// 	// bool UItemSpawnerComponent::GetRandomItemStruct(FItemStruct& OutItem)
// 	// {
// 	// 	if (!ItemDataTable) return false;
// 	//
// 	// 	TArray<FName> RowNames = ItemDataTable->GetRowNames();
// 	// 	if (RowNames.Num() == 0) return false;
// 	//
// 	// 	FName RandomRowName = RowNames[FMath::RandRange(0, RowNames.Num() - 1)];
// 	// 	static const FString ContextString(TEXT("Item Spawn Lookup"));
// 	//
// 	// 	// 데이터 테이블에서 해당 행을 찾음
// 	// 	FItemStruct* FoundItem = ItemDataTable->FindRow<FItemStruct>(RandomRowName, ContextString);
// 	//
// 	// 	// 아이템을 찾지 못한 경우 실패
// 	// 	if (!FoundItem) return false;
// 	//
// 	// 	// 찾은 아이템 데이터를 반환할 `OutItem`에 복사
// 	// 	OutItem = *FoundItem;
// 	// 	return true; // 정상적으로 아이템을 가져왔으면 `true` 반환
// 	// }
// }

