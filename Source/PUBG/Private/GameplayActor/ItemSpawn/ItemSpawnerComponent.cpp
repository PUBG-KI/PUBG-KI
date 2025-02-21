// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/ItemSpawn/ItemSpawnerComponent.h"

#include "BaseLibrary/DataEnum/ItemEnum.h"
#include "Weapon/DataTable/DT_Weapon.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "Item/ItemBase.h"

// Sets default values for this component's properties
UItemSpawnerComponent::UItemSpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UItemSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();


	
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	//SpawnItems();
	// ...
	
}


// Called every frame
void UItemSpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//아이템 스폰 함수
void UItemSpawnerComponent::SpawnItem(FName ItemID,FVector SpawnLocation)
{
	UWorld* World = GetWorld();

	//for (const FVector& SpawnLocation : SpawnLocations)
	{
		//아이템 랜덤 생성
		AItemBase* SpawnedItem = World->SpawnActor<AItemBase>(BP_Item,SpawnLocation,FRotator::ZeroRotator);
			
		SpawnedItem->SetRandomProperties(ItemID);
	}
	
}

void UItemSpawnerComponent::SpawnItems()
{
	AActor* Owner = GetOwner();
	
	//랜덤 위치 랜덤 아이템 스폰
	for (const FVector& SpawnLocation : SpawnLocations)
	{
		for (int i = 0; i < SpawnItemCount; i++)
		{
			FVector WorldSpawnLocation = Owner->GetTransform().TransformPosition(SpawnLocation);
			FVector FinalLocation = WorldSpawnLocation + GetRandomOffset();
			
			//스폰시킬 아이템 정하기
			FName SpawnedItemName = GetRandomItemRowName();
			UE_LOG(LogTemp, Warning, TEXT("RandomRowName: %s") , *SpawnedItemName.ToString());

			SpawnItem(SpawnedItemName,FinalLocation);
			
			//랜덤값이 무기인지 확인
			if (IsWeapon(SpawnedItemName))
			{
				//UE_LOG(LogTemp, Warning, TEXT("RandomRowName: %s") , *SpawnedItemName.ToString());
					
				//무기 정보 테이블 접근
				FWeaponData* WeaponData = WeaponTable->FindRow<FWeaponData>(SpawnedItemName, TEXT("Weapon Info Lookup"));
	
				if (WeaponData)
				{
					FName BulletTypeName = GetBulletTypeName(WeaponData->BulletType);
					UE_LOG(LogTemp, Warning, TEXT("BulletTypeName: %s") , *BulletTypeName.ToString());
					
					//무기에 맞는 총알 찾기
					FItemStruct* BulletData = SpawnItemTable->FindRow<FItemStruct>(BulletTypeName, TEXT("Ammo Lookup"));

					if (BulletData)
					{
						for (int BulletCount = 0; BulletCount < 2; BulletCount++)
						{
							UE_LOG(LogTemp, Warning, TEXT("BulletCount: %d") , BulletCount);
							
							FVector BulletRandomOffset = GetRandomOffset();
							
							SpawnItem(BulletTypeName,FinalLocation + BulletRandomOffset);
						}//for
					}//if
				}//if
			}//if
		}//for
	}//for	
}

//랜덤 아이템 지정
FName UItemSpawnerComponent::GetRandomItemRowName()
{
	TArray<FName> RowNames = SpawnItemTable->GetRowNames();
	FName RandomRowName = RowNames[FMath::RandRange(0,RowNames.Num()-1)];

	//UE_LOG(LogTemp, Warning, TEXT("RandomRowName: %s") , *RandomRowName.ToString());
	
	return RandomRowName;
}

// //스폰 시 속성 변경
// void UItemSpawnerComponent::SetRandomProperties(AItemBase* Item, FName ItemRowName)
// {
// 	static const FString ContextString(TEXT("Item Lookup"));
// 	FItemStruct* FoundItem = SpawnItemTable->FindRow<FItemStruct>(ItemRowName, ContextString);
//
// 	UItemDataComponent* ItemDataComponent = Item->GetItemDataComponent();
//
// 	if (FoundItem)
// 	{
// 		//아이디 설정
// 		ItemDataComponent->SetItemID(SpawnItemTable,ItemRowName);
// 		UE_LOG(LogTemp, Warning, TEXT("RandomRowName: %s") , *ItemRowName.ToString());
// 		
// 	}
//
// 	if (FoundItem && FoundItem->StaticMesh)
// 	{
// 		//메쉬변경
// 		Item->SetMesh(FoundItem->StaticMesh);
// 	}
//
// 	if (FoundItem && FoundItem->Weight)
// 	{
// 		//무게 변경
// 		ItemDataComponent->SetItemWeight(FoundItem->Weight);
// 	}
//
// 	if (FoundItem && FoundItem->Quantity)
// 	{
// 		//수량 변경
// 		ItemDataComponent->SetItemQuantity(FoundItem->Quantity);
// 	}
// }

//무기 타입인지 확인
bool UItemSpawnerComponent::IsWeapon(FName ItemID)
{
	static const FString ContextString(TEXT("Item Lookup"));
	FItemStruct* FoundItem = SpawnItemTable->FindRow<FItemStruct>(ItemID, ContextString);

	return (FoundItem && FoundItem->ItemType == EItemType::GunWeapon);
}

//총알 타입 찾기
FName UItemSpawnerComponent::GetBulletTypeName(EBulletType BulletType)
{
	switch (BulletType)
	{
		case EBulletType::B_7_62: return FName("7.62mm");
		case EBulletType::B_5_56: return FName("5.56mm");
		case EBulletType::B_9: return FName("9mm");
		case EBulletType::B_12G: return FName("12G");
		default: return FName("Unknown");
	}
}

//랜덤 반경
FVector UItemSpawnerComponent::GetRandomOffset()
{
	return FVector(
		FMath::RandRange(-ItemSpawnRadius, ItemSpawnRadius),
		FMath::RandRange(-ItemSpawnRadius, ItemSpawnRadius),
		0.0f
	);
}
	
//스폰시킬 랜덤 위치 지정
// FVector UItemSpawnerComponent::GetSpawnlocation()
// {
// 	AActor* Owner = GetOwner();
//
// 	//랜덤 위치 랜덤 아이템 스폰
// 	for (const FVector& SpawnLocation : SpawnLocations)
// 	{
// 		FVector WorldSpawnLocation = Owner->GetTransform().TransformPosition(SpawnLocation);
// 		FVector FinalLocation = WorldSpawnLocation + GetRandomOffset();
// 	}
//
// 	return FinalLocation;
// }

