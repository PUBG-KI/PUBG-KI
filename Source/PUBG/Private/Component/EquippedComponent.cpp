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
	//EItemCategory ItemCategory = GetEquippedItemCategory(Item);
	int32 ItemCategory = static_cast<int>(GetEquippedItemCategory(Item));

	// 2. 카테고리에 따라 장착 
	switch (ItemCategory)
	{
	case 0: // MainWeapon
		{
			// 1. 빈 슬롯 찾기i

			AWeaponItem* MainWeapon = Cast<AWeaponItem>(Item);
		
			if (int32 EmptyMainWeaponIndex = FindSlotMainWeapon() != -1) 
			{
				// true면 빈 슬롯이 있는 것
				EquipMainWeapon(EmptyMainWeaponIndex, MainWeapon);
				
			}
			else
			{
				// false면 빈 슬롯이 없는 것으로 현재 들고 있는 무기와 스왑 
			}
			
			break;
		}
	case 1: // SubWeapon
		{
			break;
		}
	case 2: // MeleeWeapon
		{
			break;
		}
	case 3: // Throw
		{
			break;
		}
	case 4: // Helmet
		{
			break;
		}
	case 5: // Bag
		{
			break;
		}
	case 6: // Vest
		{
			break;
		}
	case 7: // Bag
		{
			break;
		}
		
	}

	// 2. 카테고리에 따라 아이템 장착 

}

int32 UEquippedComponent::FindSlotMainWeapon()
{
	for (int i = 0; i < EquippedMainWeapon.Num(); i++)
	{
		// true면 빈 슬롯이 있는 것  
		if (EquippedMainWeapon[i] == nullptr)
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
		EquippedMainWeapon[InIndex] = MainWeapon;
	
		FName ItemID = MainWeapon->GetItemDataComponent()->GetItemRowName();
		FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
		
		AEquipableItem* EquipableItem = GetWorld()->SpawnActor<AEquipableItem>(Row->BP_Item);
	
		if (AWeapon_Base* Weapon = Cast<AWeapon_Base>(EquipableItem))
		{
			FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			
			if (InIndex == 1)
			{
				PrimarySlot = Weapon;
				// 필요한 값 넣기 MainWeapon -> Weapon
		
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
				{
					PrimarySlot->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_primarySocket")));
		
				}
				
			}
			else if (InIndex == 2)
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
	
	// 빈 슬롯인지 한번 더 확인
	if (EquippedMainWeapon[InIndex] != nullptr)
	{
		// 빈 슬롯이 아닌 것, 교체해야 됨
	}
	else
	{
		// 빈 슬롯으로, 그냥 장착
		EquippedMainWeapon[InIndex] = MainWeapon;
	
		FName ItemID = MainWeapon->GetItemDataComponent()->GetItemRowName();
		FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
		
		AEquipableItem* EquipableItem = GetWorld()->SpawnActor<AEquipableItem>(Row->BP_Item);
	
		if (AWeapon_Base* Weapon = Cast<AWeapon_Base>(EquipableItem))
		{
			FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			
			if (InIndex == 0)
			{
				PrimarySlot = Weapon;
				// 필요한 값 넣기 MainWeapon -> Weapon
		
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
				{
					UE_LOG(LogTemp, Warning, TEXT("EquippedComponent : Player AttachToComponent"));
					PrimarySlot->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_primarySocket")));
					//EquipableItem->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_primarySocket")));
		
				}
				
			}
			else if (InIndex == 1)
			{
				SecondarySlot = Weapon;
				
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
				{
					SecondarySlot->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_secondarySocket")));
				}
			}
		}
			

		// 장착된 무기 스폰
	}
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






