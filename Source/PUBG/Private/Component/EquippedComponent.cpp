// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EquippedComponent.h"

#include "BaseLibrary/DataEnum/ItemEnum.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "Item/ItemBase.h"
#include "Item/WeaponItem.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UEquippedComponent::UEquippedComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	
	FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");
	
	ItemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));

	EquippedMainWeapon.SetNum(2);
	EquippedItems.SetNum(16);
}

void UEquippedComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquippedComponent, CurrentWeapon);
	DOREPLIFETIME(UEquippedComponent, LastCurrentWeapon);
	DOREPLIFETIME(UEquippedComponent, PrimarySlot);
	DOREPLIFETIME(UEquippedComponent, SecondarySlot);
}


// Called when the game starts
void UEquippedComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UEquippedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

EItemCategory UEquippedComponent::GetEquippedItemCategory(AItemBase* Item)
{
	FName ItemID = Item->GetItemDataComponent()->GetItemRowName();

	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));

	return Row->Category;
}

void UEquippedComponent::EquipItem(AItemBase* Item)
{
	// 1. 아이템 카테고리 찾기
	EItemCategory ItemCategory = GetEquippedItemCategory(Item);
	//int32 ItemCategory = static_cast<int>(GetEquippedItemCategory(Item));

	FName ItemID = Item->GetItemDataComponent()->GetItemRowName();
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
		
	AEquipableItem* EquipableItem = GetWorld()->SpawnActor<AEquipableItem>(Row->BP_Item);
	

	
	

}

int32 UEquippedComponent::FindSlotMainWeapon()
{
	for (int i = 0; i < 2; i++)
	{
		// true면 빈 슬롯이 있는 것  
		if (EquippedItems[i] == nullptr)
		{
			return i;
		}
	}

	// 빈 슬롯이 없는 것 
	return -1;
}

void UEquippedComponent::ServerEquipMainItem_Implementation(AItemBase* Item)
{
	AWeaponItem* WeaponItem = Cast<AWeaponItem>(Item);

	FName ItemID = WeaponItem->GetItemDataComponent()->GetItemRowName();
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
	
	AGun_Base* MainWeapon = Cast<AGun_Base>(GetWorld()->SpawnActor<AEquipableItem>(Row->BP_Item));
	FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	
	int32 RemainSlot = FindSlotMainWeapon();
	
	int32 MainWeaponSlot = static_cast<int32>(MainWeapon->GetEquipSlot()); // 장착될 슬롯값 가져오기
	
	if (RemainSlot == -1) // Main 무기 슬롯이 가득찬 것 
	{
		int32 DropMainWeaponSlot = DropMainWeapon();
		RemainSlot = DropMainWeaponSlot;
	}
	
	if (RemainSlot == 0) // 1번 슬롯이 비어있는 것 
	{
		EquippedItems[MainWeaponSlot] = MainWeapon;
		EquippedItems[MainWeaponSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_primarySocket")));

		// 필요한 값 넣기 MainWeapon -> Weapon
	}
	else if (RemainSlot == 1) // 2번 슬롯이 비어있는 것 
	{
		MainWeapon->SetEquipSlot(EEquippedItemCategory::SecondarySlot);
		MainWeaponSlot = static_cast<int32>(MainWeapon->GetEquipSlot());
		
		
		EquippedItems[MainWeaponSlot] = MainWeapon;
		EquippedItems[MainWeaponSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_secondarySocket")));
	}
}

int32 UEquippedComponent::DropMainWeapon(AGun_Base* OutCurrentWeapon)
{
	// 버리는 것은 무조건 UI에서만, 지정해서
	// 교체할 때 쓰기 위해서는 버릴 아이템을 매개변수로 
	
	if (OutCurrentWeapon == nullptr) // 매개변수가 없다는 것은 교체할 때 쓰는 것 CurrentWeapon과 2번 슬롯의 무기 
	{
		
		if (CurrentWeapon != nullptr) // 현재 손에 든 무기가 있으면 현재 무기와 교체
		{
			ServerSpawnStaticMesh(Cast<AGun_Base>(CurrentWeapon));
			
			int32 DropIndex = static_cast<int32>(CurrentWeapon->GetEquipSlot());
			UE_LOG(LogTemp, Warning, TEXT("Drop Index : %d"), DropIndex);

			CurrentWeapon->Destroy();
			CurrentWeapon = nullptr;
			return DropIndex;
		}
		else // 현재 손에 든 무기가 없으면 2번 슬롯 무기와 교체
		{
			AGun_Base* MainWeapon = Cast<AGun_Base>(EquippedItems[1]);
			ServerSpawnStaticMesh(MainWeapon);
			
			FName ItemID = FName(Cast<AGun_Base>(EquippedItems[1])->GetWeaponDataAsset().GunName); // RowName 가져오기
			int32 RowIndex = GetRowIndex(ItemDataTable, ItemID); // RowName에 해당하는 인덱스 가져오기
			UE_LOG(LogTemp, Warning, TEXT("Row Index : %d"), RowIndex);
			
			int32 DropIndex = static_cast<int32>(EquippedItems[1]->GetEquipSlot());
			UE_LOG(LogTemp, Warning, TEXT("Drop Index : %d"), DropIndex);

			EquippedItems[1]->Destroy(); 
			EquippedItems[1] = nullptr;
			return DropIndex;

		}
	}
	else
	{
		ServerSpawnStaticMesh(Cast<AGun_Base>(OutCurrentWeapon));

		int32 DropIndex = static_cast<int32>(OutCurrentWeapon->GetEquipSlot());
		OutCurrentWeapon = nullptr;
		return DropIndex;
	}
}

void UEquippedComponent::ServerSpawnStaticMesh_Implementation(AGun_Base* OutCurrentWeapon)
{
	FName ItemID = FName(OutCurrentWeapon->GetWeaponDataAsset().GunName); // RowName 가져오기
			
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row")); // 테이블 가져오기
	int32 RowIndex = GetRowIndex(ItemDataTable, ItemID); // RowName에 해당하는 인덱스 가져오기 
	UE_LOG(LogTemp, Warning, TEXT("Row Index : %d"), RowIndex);

			
	FRotator SpawnRotation = FRotator(0, 0, 0);
	FVector SpawnLocation = DropLocation();
	UE_LOG(LogTemp, Warning, TEXT("SpawnLocation : %f %f %f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);

	UClass* MainWeaponItemBPClass = LoadClass<AWeaponItem>(nullptr, TEXT("/Game/Blueprint/Item/Farming/TestWeaponItem.TestWeaponItem_C"));
	if (MainWeaponItemBPClass) // nullptr 체크 추가
	{
		if (AWeaponItem* TempWeapon = GetWorld()->SpawnActorDeferred<AWeaponItem>(MainWeaponItemBPClass, FTransform(SpawnRotation, SpawnLocation)))
		{
			//TempWeapon->SetTableIndex(RowIndex);
			TempWeapon->SetTableIndex(-1);
			TempWeapon->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
			UE_LOG(LogTemp, Warning, TEXT("TempWeapon GetItemRowName : %s"), *TempWeapon->GetItemDataComponent()->GetItemRowName().ToString());

		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load blueprint class! Check the path: /Game/Blueprint/Item/Farming/TestWeaponItem.TestWeaponItem_C"));
	}
}


int32 UEquippedComponent::GetRowIndex(UDataTable* DataTable, FName TargetRowName)
{
	if (DataTable == nullptr)
	{
		return -1;
	}

	TArray<FName> RowNames = DataTable->GetRowNames(); // 모든 RowName 가져오기

	for (int32 Index = 0; Index < RowNames.Num(); Index++)
	{
		if (RowNames[Index] == TargetRowName)
		{
			return Index; // 일치하는 인덱스 반환
		}
	}

	return -1;
}

FVector UEquippedComponent::DropLocation()
{
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : DropLocation"));
		
		FVector Start = GetOwner()->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("Start Location : %f %f %f"), Start.X, Start.Y, Start.Z);
		FVector End = GetOwner()->GetActorLocation() - FVector(0.0f, 0.0f, 500.0f);
		UE_LOG(LogTemp, Warning, TEXT("End Location : %f %f %f"), End.X, End.Y, End.Z);
		ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_WorldDynamic);
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(GetOwner());
		FHitResult Hit;

		//UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceChannel, false, IgnoreActors, EDrawDebugTrace::Persistent, Hit, true, FLinearColor::Red, FLinearColor::Green);

		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceChannel, false, IgnoreActors, EDrawDebugTrace::Persistent, Hit, true, FLinearColor::Red, FLinearColor::Green))
		{
			UE_LOG(LogTemp, Warning, TEXT("Drop Location : %f %f %f"), Hit.Location.X, Hit.Location.Y, Hit.Location.Z);
			return Hit.Location;
		}

		UE_LOG(LogTemp, Warning, TEXT("Drop Location : 0"));
		return FVector(0, 0, 0);	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : DropLocation"));

		FVector Start = GetOwner()->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("Start Location : %f %f %f"), Start.X, Start.Y, Start.Z);
		FVector End = GetOwner()->GetActorLocation() - FVector(0.0f, 0.0f, 500.0f);
		UE_LOG(LogTemp, Warning, TEXT("End Location : %f %f %f"), End.X, End.Y, End.Z);
		ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_WorldDynamic);
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(GetOwner());
		FHitResult Hit;

		UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceChannel, false, IgnoreActors, EDrawDebugTrace::Persistent, Hit, true, FLinearColor::Red, FLinearColor::Green);

		// if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceChannel, false, IgnoreActors, EDrawDebugTrace::Persistent, Hit, true, FLinearColor::Red, FLinearColor::Green))
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("Drop Location : %f %f %f"), Hit.Location.X, Hit.Location.Y, Hit.Location.Z);
		// 	return Hit.Location;
		// }

		UE_LOG(LogTemp, Warning, TEXT("Drop Location : 0"));
		return FVector(0, 0, 0);
	}
	
	return FVector(0, 0, 0);
}

void UEquippedComponent::SetCurrentWeapon(AWeapon_Base* _CurrentWeapon)
{
	this->CurrentWeapon = _CurrentWeapon;
}

void UEquippedComponent::SetLastCurrentWeapon(AWeapon_Base* _LastCurrentWeapon)
{
	this->LastCurrentWeapon = _LastCurrentWeapon;
}

void UEquippedComponent::SetPrimarySlotWeapon(AWeapon_Base* _PrimarySlot)
{
	this->PrimarySlot = _PrimarySlot;
}

void UEquippedComponent::SetSecondarySlotWeapon(AWeapon_Base* _Secondary)
{
	this->SecondarySlot = _Secondary;
}

void UEquippedComponent::SetSideArmSlotWeapon(AWeapon_Base* _SideArm)
{
	this->SideArmSlot = _SideArm;
}

void UEquippedComponent::SetMeleeSlotWeapon(AWeapon_Base* _Melee)
{
	this->MeleeSlot = _Melee;
}

void UEquippedComponent::SetThrowableSlotWeapon(AWeapon_Base* _Throwable)
{
	this->ThrowableSlot = _Throwable;
}






