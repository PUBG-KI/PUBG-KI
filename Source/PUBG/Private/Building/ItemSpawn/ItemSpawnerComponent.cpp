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
				UE_LOG(LogTemp, Warning, TEXT("Spawned"));

				FName SpawnedItemName = GetRandomItemRowName();
				
				SetRandomProperties(SpawnedItem,SpawnedItemName);
				
				UE_LOG(LogTemp, Warning, TEXT("ItemRowName(): %s") , *SpawnedItem->GetItemDataComponent()->GetItemRowName().ToString());
				
				
				//TArray<FName> RowNames = ItemDataComponent;
			}
		}
		
	}
}

void UItemSpawnerComponent::SetRandomMesh(AItemBase* Item, FName ItemRowName)
{
	// static const FString ContextString(TEXT("Item Lookup"));
	// FItemStruct* FoundItem = SpawnItemTable->FindRow<FItemStruct>(ItemRowName, ContextString);
	//
	// UE_LOG(LogTemp, Warning, TEXT("ItemRowName: %s") , *ItemRowName.ToString());
	//
	// if (FoundItem && FoundItem->StaticMesh)
	// {
	// 	Item->SetMesh(FoundItem->StaticMesh);
	// }
}

FName UItemSpawnerComponent::GetRandomItemRowName()
{
	TArray<FName> RowNames = SpawnItemTable->GetRowNames();
	FName RandomRowName = RowNames[FMath::RandRange(0,RowNames.Num()-1)];

	UE_LOG(LogTemp, Warning, TEXT("RandomRowName: %s") , *RandomRowName.ToString());
	
	return RandomRowName;
}

void UItemSpawnerComponent::SetRandomProperties(AItemBase* Item, FName ItemRowName)
{
	static const FString ContextString(TEXT("Item Lookup"));
	FItemStruct* FoundItem = SpawnItemTable->FindRow<FItemStruct>(ItemRowName, ContextString);

	UItemDataComponent* ItemDataComponent = Item->GetItemDataComponent();
	
	UE_LOG(LogTemp, Warning, TEXT("ItemRowName: %s") , *ItemRowName.ToString());

	if (FoundItem)
	{
		//아이디 설정
		ItemDataComponent->SetItemID(ItemRowName);	
	}

	if (FoundItem && FoundItem->StaticMesh)
	{
		//메쉬변경
		Item->SetMesh(FoundItem->StaticMesh);
	}

	if (FoundItem && FoundItem->Weight)
	{
		//무게 변경
		ItemDataComponent->SetItemWeigt(FoundItem->Weight);
	}

	if (FoundItem && FoundItem->Quantity)
	{
		//수량 변경
		ItemDataComponent->SetItemQuantity(FoundItem->Quantity);
	}

	UE_LOG(LogTemp, Warning, TEXT("Spawned Item: %s | Mesh: %s | Weight: %.2f | Quantity: %d"),
			*ItemRowName.ToString(),
			FoundItem->StaticMesh ? *FoundItem->StaticMesh->GetName() : TEXT("None"),
			FoundItem->Weight,
			FoundItem->Quantity);
}

//SetItem 하기 위함
// bool UItemSpawnerComponent::SetItemStruct(FItemStruct& Output)
// {
// 	if (!SpawnItemTable)
// 	{
// 		return false;
// 	}
// 	
// 	TArray<FName> RowNames = SpawnItemTable->GetRowNames();
// 	FName RandomRowName = RowNames[FMath::RandRange(0,RowNames.Num()-1)];
//
// 	//UE_LOG(LogTemp, Warning, TEXT("CurrentLocation: %s") , *CurrentLocation.ToString());
// 	UE_LOG(LogTemp, Warning, TEXT("RandomRowName: %s") , *RandomRowName.ToString());
// 	static const FString ContextString(TEXT("Item Spawn Lookup"));
// 	
// 	FItemStruct* FoundItem = SpawnItemTable->FindRow<FItemStruct>(RandomRowName,ContextString);
//
// 	
// 	if (!FoundItem)
// 	{
// 		return false;
// 	}
//
// 	Output = *FoundItem;	
// 	
// 	return true;
// }

//아이템 아이디 랜덤
