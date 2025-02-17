// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/ItemSpawn/ItemSpawnerComponent.h"
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

	//bool bNextShouldBeAmmo = false;
	
	for (const FVector& SpawnLocation : SpawnLocations)
	{
		//for (int32 i = 0; i < SpawnItemCount; i++)
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

				//랜덤값이 무기인지 확인
				if (IsWeapon(SpawnedItemName))
				{
					//bNextShouldBeAmmo = true;
					
					// //SetRandomProperties(SpawnedItem)
					// // SetRandomProperties(SpawnedItem,SpawnedItemName);
					// //SetRandomProperties(SpawnedItem,SpawnedItemName);
					// SpawnWeapon(SpawnedItemName,FinalLocation);
					// UE_LOG(LogTemp, Warning, TEXT("SpawnWeapon(): %s") , *SpawnedItemName.ToString());

					//무기 정보 테이블에서 해당 총기의 탄약 정보 가져오기
					FWeaponData* WeaponData = WeaponTable->FindRow<FWeaponData>(SpawnedItemName, TEXT("Weapon Info Lookup"));
					if (!WeaponData)
					{
						return;
					}
					//AItemBase* SpawnedWeapon = GetWorld()->SpawnActor<AItemBase>(BP_Item, SpawnLocation, FRotator::ZeroRotator);

					if (WeaponData)
					{
						SetRandomProperties(SpawnedItem, SpawnedItemName);
					}

					FName BulletTypeName = GetBulletTypeName(WeaponData->BulletType);
					FItemStruct* AmmoData = SpawnItemTable->FindRow<FItemStruct>(BulletTypeName, TEXT("Ammo Lookup"));

					if (AmmoData)
					{
						// 탄약 스폰할 위치 계산 (총기 근처)
						AItemBase* SpawnedAmmo = GetWorld()->SpawnActor<AItemBase>(BP_Item, FinalLocation,FRotator::ZeroRotator);
						
						if (SpawnedAmmo)
						{
							SetRandomProperties(SpawnedAmmo, BulletTypeName);
						}
					}
				}
				else
				{
					SetRandomProperties(SpawnedItem,SpawnedItemName);
				}
			}
		}
		
	}
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

	// UE_LOG(LogTemp, Warning, TEXT("Spawned Item: %s | Mesh: %s | Weight: %.2f | Quantity: %d"),
	// 		*ItemRowName.ToString(),
	// 		FoundItem->StaticMesh ? *FoundItem->StaticMesh->GetName() : TEXT("None"),
	// 		FoundItem->Weight,
	// 		FoundItem->Quantity);
}

bool UItemSpawnerComponent::IsWeapon(FName ItemID)
{
	static const FString ContextString(TEXT("Weapon Lookup"));
	FWeaponData* FoundWeapon = WeaponTable->FindRow<FWeaponData>(ItemID, ContextString);

	if ( WeaponTable && FoundWeapon != nullptr)
	{
		return true;
	}
	return false;
}

// void UItemSpawnerComponent::SpawnWeapon(FName WeaponID , FVector SpawnLocation)
// {
// 	FItemStruct* ItemData = SpawnItemTable->FindRow<FItemStruct>(WeaponID, TEXT("Item Lookup"));
// 	if (!ItemData)
// 	{
// 		return;
// 	}
//
// 	//무기 정보 테이블에서 해당 총기의 탄약 정보 가져오기
// 	FWeaponData* WeaponData = WeaponTable->FindRow<FWeaponData>(WeaponID, TEXT("Weapon Info Lookup"));
// 	if (!WeaponData)
// 	{
// 		return;
// 	}
//
// 	FName BulletTypeName = GetBulletTypeName(WeaponData->BulletType);
// 	FItemStruct* AmmoData = SpawnItemTable->FindRow<FItemStruct>(BulletTypeName, TEXT("Ammo Lookup"));
// 	
// 	AItemBase* SpawnedWeapon = GetWorld()->SpawnActor<AItemBase>(BP_Item, SpawnLocation, FRotator::ZeroRotator);
//
// 	if (SpawnedWeapon)
// 	{
// 		SetRandomProperties(SpawnedWeapon, WeaponID);
// 	}
// 	
// 	if (AmmoData)
// 	{
// 		// 탄약 스폰할 위치 계산 (총기 근처)
// 		AItemBase* SpawnedAmmo = GetWorld()->SpawnActor<AItemBase>(BP_Item, SpawnLocation, FRotator::ZeroRotator);
//
// 		if (SpawnedAmmo)
// 		{
// 			SetRandomProperties(SpawnedAmmo, BulletTypeName);
// 		}
// 	}
// }

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

// FVector UItemSpawnerComponent::GetRandomSpawnLocation(FVector BaseLocation, float MinOffset, float MaxOffset)
// {
// 	UWorld* World = GetWorld();
// 	AActor* Owner = GetOwner();
//
// 	for (const FVector& SpawnLocation : SpawnLocations)
// 	{
// 		for (int32 i = 0; i < SpawnItemCount; i++)
// 		{
// 			FVector WorldSpawnLocation = Owner->GetTransform().TransformPosition(SpawnLocation);
//
// 			//위치 주변 배치
// 			FVector RandomOffset = FVector(
// 				FMath::RandRange(-ItemSpawnRadius, ItemSpawnRadius),
// 				FMath::RandRange(-ItemSpawnRadius, ItemSpawnRadius),
// 				0.0f
// 			);
// 			
// 			FVector FinalLocation = WorldSpawnLocation + RandomOffset;
//
// 			return FinalLocation;
// }
