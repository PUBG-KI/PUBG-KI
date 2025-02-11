// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EquippedComponent.h"

#include "Component/ItemData/ItemDataComponent.h"
#include "Item/ItemBase.h"
#include "Item/WeaponItem.h"

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
	int32 ItemCategory = static_cast<int>(GetEquippedItemCategory(Item));

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

void UEquippedComponent::EquipMainWeapon(int32 InIndex, AWeaponItem* MainWeapon)
{
	UE_LOG(LogTemp, Warning, TEXT("EquipMainWeapon"));

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : EquipMainWeapon"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : EquipMainWeapon"));
	}
	
	// 빈 슬롯인지 한번 더 확인
	if (EquippedMainWeapon[InIndex] != nullptr)
	{
		// 빈 슬롯이 아닌 것, 교체해야 됨
	}
	else
	{
		// 빈 슬롯으로, 그냥 장착
		//EquippedMainWeapon[InIndex] = MainWeapon;
	
		FName ItemID = MainWeapon->GetItemDataComponent()->GetItemRowName();
		FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
		
		AEquipableItem* EquipableItem = GetWorld()->SpawnActor<AEquipableItem>(Row->BP_Item);
	
		if (AWeapon_Base* Weapon = Cast<AWeapon_Base>(EquipableItem))
		{
			FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			
			if (InIndex == 0)
			{
				//PrimarySlot = Weapon;
				//EquippedItems[InIndex] = EquipableItem;
				EquippedItems[InIndex] = Weapon;
				// 필요한 값 넣기 MainWeapon -> Weapon
		
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
				{
					PrimarySlot->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_primarySocket")));
				}
				
			}
			else if (InIndex == 1)
			{
				SecondarySlot = Weapon;
				
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
				{
					SecondarySlot->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_primarySocket")));
				}
			}
			
		}
			

		// 장착된 무기 스폰
	}
}

void UEquippedComponent::ServerEquipMainItem_Implementation(AItemBase* Item)
{
	AWeaponItem* WeaponItem = Cast<AWeaponItem>(Item);

	FName ItemID = WeaponItem->GetItemDataComponent()->GetItemRowName();
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
	
	AWeapon_Base* MainWeapon = Cast<AWeapon_Base>(GetWorld()->SpawnActor<AEquipableItem>(Row->BP_Item));
	FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	
	int32 RemainSlot = FindSlotMainWeapon();
	//int32 EnumValue = static_cast<int32>(RemainSlot); 
	int32 EnumValue = static_cast<int32>(static_cast<EEquippedItemCategory>(RemainSlot));
	
	if (RemainSlot == -1) // Main 무기 슬롯이 가득찬 것 
	{

		// 무기 교체 
	}
	else if (RemainSlot == 0) // 1번 슬롯이 비어있는 것 
	{
		EquippedItems[RemainSlot] = MainWeapon;
		EquippedItems[RemainSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_primarySocket")));

		// 필요한 값 넣기 MainWeapon -> Weapon
		
	}
	else if (RemainSlot == 1) // 2번 슬롯이 비어있는 것 
	{
		EquippedItems[RemainSlot] = MainWeapon;
		EquippedItems[RemainSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_secondarySocket")));
	}
}

void UEquippedComponent::SwapMainWeapon(AWeaponItem* MainWeapon)
{
	// 0. 아이템이 들어오면 MainWeapon의 이름을 통해 데이터 테이블에 접근하여 블루프린트 생성 후 교체할 아이템은 WeaponItem으로 바꾸어서 드랍되게 
	if (CurrentWeapon != nullptr) // 1. 현재 손에 무기를 들고 있으면 ( CurrentWeapon이 있으면 ) 그것과 교체
	{
		for (int i = 0; i<EquippedMainWeapon.Num(); i++)
		{
			// if (EquippedMainWeapon[i] == MainWeapon)
			// {
			// 	// 1.1 장착 중인 무기를 바닥에 버림
			// }
		}
	}
	else // 2. 맨손이고 슬롯이 가득 차 있으면 무조건 2번 무기와 교체
	{
		
	}
}

void UEquippedComponent::ServerEquipItem_Implementation(int32 InIndex, AWeaponItem* MainWeapon)
{
	UE_LOG(LogTemp, Warning, TEXT("EquipMainWeapon"));

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : EquipMainWeapon"));

		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : EquipMainWeapon"));
	}
	
	// // 빈 슬롯인지 한번 더 확인
	// if (EquippedMainWeapon[InIndex] != nullptr)
	// {
	// 	// 빈 슬롯이 아닌 것, 교체해야 됨
	// }
	// else
	// {
	// 	// 빈 슬롯으로, 그냥 장착
	// 	EquippedMainWeapon[InIndex] = MainWeapon;
	//
	// 	FName ItemID = MainWeapon->GetItemDataComponent()->GetItemRowName();
	// 	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
	// 	
	// 	AEquipableItem* EquipableItem = GetWorld()->SpawnActor<AEquipableItem>(Row->BP_Item);
	//
	// 	if (AWeapon_Base* Weapon = Cast<AWeapon_Base>(EquipableItem))
	// 	{
	// 		FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	// 		
	// 		if (InIndex == 0)
	// 		{
	// 			PrimarySlot = Weapon;
	// 			// 필요한 값 넣기 MainWeapon -> Weapon
	// 	
	// 			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
	// 			{
	// 				UE_LOG(LogTemp, Warning, TEXT("EquippedComponent : Player AttachToComponent"));
	// 				PrimarySlot->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_primarySocket")));
	// 				//EquipableItem->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_primarySocket")));
	// 	
	// 			}
	// 			
	// 		}
	// 		else if (InIndex == 1)
	// 		{
	// 			SecondarySlot = Weapon;
	// 			
	// 			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
	// 			{
	// 				SecondarySlot->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_secondarySocket")));
	// 			}
	// 		}
	// 	}
	// 	
	// }
	// MainWeapon->Destroy(true);
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






