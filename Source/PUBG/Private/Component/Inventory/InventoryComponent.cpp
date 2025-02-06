// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Inventory/InventoryComponent.h"

#include "AudioMixerBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "Character/TestCharacter.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "Interface/InteractInterface.h"
#include "Item/ItemBase.h"
//#include "Math/UnrealMathNeon.h"
#include "Controller/BasePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/Inventory/InventoryWidget.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
	
	MaxInventoryWeight = 50.0f;
	CurrentInventoryWeight = 0.0f;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//Content.SetNum(1);

	// for (FItemSlotStruct& Slot : Content)
	// {
	// 	Slot.ItemName = NAME_None;
	// 	Slot.Quantity = 0;
	// 	Slot.Tag = FGameplayTag(); // 빈 태그로 초기화
	// }
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UInventoryComponent, Content);
	
	DOREPLIFETIME_CONDITION(UInventoryComponent, Content, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UInventoryComponent, CurrentWeapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UInventoryComponent, NearItem, COND_OwnerOnly);
	
}

void UInventoryComponent::ServerSetNearItem_Implementation(AItemBase* OutNearItem)
{
    SetNearItem(OutNearItem);
}



void UInventoryComponent::Server_InteractItem_Implementation(AItemBase* OutItemBase)
{
	if (!Item && !NearItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item, NearItem None"));
		return;
	}
	
	if (Item != nullptr)
	{
		SetItem(OutItemBase);
		UItemDataComponent* ItemDataComponent = Item->GetItemDataComponent();
		ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());

		AActor* Owner = GetOwner();
		if (APlayerCharacter* Character = Cast<APlayerCharacter>(Owner))
		{
			ItemDataComponent->InteractWith(Character);
		}
	}
	
	if (NearItem != nullptr)
	{
		SetNearItem(NearItem);
		UE_LOG(LogTemp, Warning, TEXT("NearItem"));
		
		UItemDataComponent* ItemDataComponent = NearItem->GetItemDataComponent();
		//ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());

		AActor* Owner = GetOwner();
		if (APlayerCharacter* Character = Cast<APlayerCharacter>(Owner))
		{
			ItemDataComponent->InteractWith(Character);
		}
	}

	return;
}

bool UInventoryComponent::Server_InteractItem_Validate(AItemBase* OutItemBase)
{
	return true;
}

void UInventoryComponent::ServerSetContents_Implementation(const TArray<FItemSlotStruct>& OutContnets)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : SetContents "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : SetContents "));
	}
	
	SetContent(OutContnets);
}

int32 UInventoryComponent::AddToInventory(FName ItemID, int32 Quantity, int32 Weight)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : AddToInventory "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : AddToInventory "));
	}
	
	int32 RemainingQuantity = Quantity;
	
	// 인벤이 가득 찼는지 검사, 현재 무게와 최대 무게 비교 false면 인벤에 넣을 수 있음 
	bool IsFull = CurrentInventoryWeight + Weight > MaxInventoryWeight;
	
	for (int32 i = RemainingQuantity; i > 0 && !IsFull; i--, RemainingQuantity--)
	{
		IsFull = CurrentInventoryWeight + Weight >= MaxInventoryWeight;

		int32 SlotIndex = FindItemSlot(ItemID);
		if (SlotIndex == -1)
		{
			// -1면 아이템이 없는 것으로 마지막에 추가
			AddToLastIndexNewStack(ItemID, 1, Weight);
		}
		else
		{
			// -1이 아니면 이미 인벤토리에 같은 아이템이 존재하므로 거기에 쌓음 꽉 찼을 경우 새로운 스택을 만들어야 함 
			AddToStack(SlotIndex, 1, Weight);

			//CreateNewStack(Content.LastIndex())
		}
		
	}

	//SetContent(Content);
	//ServerSetContents_Implementation(Content);
	
	UE_LOG(LogTemp, Warning, TEXT("CurrentRemainingQuantity : %d"), RemainingQuantity);
	UE_LOG(LogTemp, Warning, TEXT("CurrentInventoryWeight : %f"), CurrentInventoryWeight);
	return RemainingQuantity;
}

int32 UInventoryComponent::FindItemSlot(FName ItemID)
{
	// 아이템이 이미 있는지 확인 함수

	// 해당 아이템이 인벤토리의 여러 슬롯에 존재하는 경우 마지막에 추가해야 됨 
	int32 LastIndex = -1;
	
	for (int i = 0; i < Content.Num(); i++)
	{
		if (Content[i].ItemName == ItemID)
		{
			LastIndex = i;
		}
	}

	return LastIndex;
}

void UInventoryComponent::AddToStack(int32 Index, int32 Quantity, int32 Weight)
{
	// 현재 인덱스의 아이템 StackSize가 꽉 차면 새로운 Stack 만들기
	if (Content[Index].Quantity == GetMaxStackSize(Content[Index].ItemName))
	{
		CreateNewStack(Content[Index].ItemName, 1, Weight, Index + 1);

		return;
	}
	
	FItemSlotStruct ItemSlot;
	ItemSlot.ItemName = Content[Index].ItemName;
	ItemSlot.Quantity = Content[Index].Quantity + Quantity;
	CurrentInventoryWeight += Weight;

	Content[Index] = ItemSlot;
	
}

void UInventoryComponent::CreateNewStack(FName ItemID, int32 Quantity, int32 Weight, int32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateNewStack : %d"), Index);
	
	FGameplayTag Tag = FGameplayTag::EmptyTag;

	FItemSlotStruct ItemSlot;
	ItemSlot.ItemName = ItemID;
	ItemSlot.Quantity = Quantity;

	//Content.Insert(ItemSlot, Index);
	//Content[Index] = ItemSlot;
	
	Content.Add(ItemSlot);
	CurrentInventoryWeight += Weight;
	UpdateInventory();
	
}

void UInventoryComponent::AddToLastIndexNewStack(FName ItemID, int32 Quantity, int32 Weight)
{
	FItemSlotStruct ItemSlot;
	ItemSlot.ItemName = ItemID;
	ItemSlot.Quantity = Quantity;
	CurrentInventoryWeight += Weight;
	
	Content.Add(ItemSlot);

	UpdateInventory();
}


	


int32 UInventoryComponent::GetMaxStackSize(FName ItemID)
{
	FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");
	
	UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));
	
	FItemStruct* Row = DataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));

	return Row->StackSize;
}


void UInventoryComponent::UpdateInventory()
{
		Content.Sort([](const FItemSlotStruct& LHS, const FItemSlotStruct& RHS)
	{
			FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");
			UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));

			if (DataTable)
			{
				UE_LOG(LogTemp, Warning, TEXT("DataTable!!"));
			}

			
			FItemStruct* LHSRow = DataTable->FindRow<FItemStruct>(LHS.ItemName, TEXT("Get Item Row"));
			FItemStruct* RHSRow = DataTable->FindRow<FItemStruct>(RHS.ItemName, TEXT("Get Item Row"));

			UE_LOG(LogTemp, Warning, TEXT("LHS.ItemName: %s, RHS.ItemName: %s"), *LHS.ItemName.ToString(), *RHS.ItemName.ToString());
			
			// LHSRow 또는 RHSRow가 nullptr인 경우를 처리
		   if (!LHSRow || !RHSRow)
		   {
			   UE_LOG(LogTemp, Warning, TEXT("Row not found for one or both items!"));
			   return false;
		   }
			
			if (LHSRow->Category != RHSRow->Category)
			{
				return LHSRow->Category < RHSRow->Category;
			}
				
			if (LHS.ItemName != RHS.ItemName)
			{
				return LHS.ItemName.ToString() < RHS.ItemName.ToString();  // Name이 다르면 Name 순으로 오름차순 정렬
			}
				
			return LHS.Quantity > RHS.Quantity;  // Quantity가 다르면 Quantity 순으로 오름차순 정렬
	});
}

void UInventoryComponent::ServerUpdateInventory_Implementation()
{
	UpdateInventory();
}

void UInventoryComponent::ReplicateContent_Implementation(const TArray<FItemSlotStruct>& OutContent)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : ReplicateContent_Implementation "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerUpdateInventory_Implementation"));
	}

	Content.Add(FItemSlotStruct());
	Content.Pop();
	
	UE_LOG(LogTemp, Warning, TEXT("Execute Client : ReplicateContent_Implementation "));
}

void UInventoryComponent::PrintContents()
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("서버에서 실행됨"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("클라이언트에서 실행됨"));
	}
	
	for (int32 Index = 0; Index < Content.Num(); ++Index)
	{
		FString Msg = Content[Index].ItemName.ToString();
		int32 Quantity = Content[Index].Quantity;
		
		UE_LOG(LogTemp, Warning, TEXT("%d, %s, %d"), Index, *Msg, Quantity);
	}
}

void UInventoryComponent::ServerPrintContents_Implementation()
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("서버에서 실행됨"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("클라이언트에서 실행됨"));
	}
	
	for (int32 Index = 0; Index < Content.Num(); ++Index)
	{
		FString Msg = Content[Index].ItemName.ToString();
		int32 Quantity = Content[Index].Quantity;
		
		UE_LOG(LogTemp, Warning, TEXT("%d, %s, %d"), Index, *Msg, Quantity);
	}
}

void UInventoryComponent::TransferSlots()
{
}

void UInventoryComponent::OnRep_Content()
{
	UE_LOG(LogTemp, Warning, TEXT("Content Replicate!"));

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (PlayerCharacter)
	{
		ABasePlayerController* PlayerController = Cast<ABasePlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			if (PlayerController->GetInventoryWidget())
			{
				PlayerController->GetInventoryWidget()->UpdateInventoryWidget();
				PlayerController->GetInventoryWidget()->UpdateNearItemSlotWidget();
			}
		}
	}
}

void UInventoryComponent::SetCurrentWeapon(AWeapon_Base* _CurrentWeapon)
{
	if (_CurrentWeapon == nullptr)
	{
		this->CurrentWeapon = nullptr;
	}
	else
	{
		this->CurrentWeapon = _CurrentWeapon;
	}

}

void UInventoryComponent::Server_Interact_Implementation()
{
	// set 
	
	if (!Item && !NearItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item, NearItem None"));
		return;
	}
	
	if (Item != nullptr)
	{
		UItemDataComponent* ItemDataComponent = Item->GetItemDataComponent();
		ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());

		AActor* Owner = GetOwner();
		if (APlayerCharacter* Character = Cast<APlayerCharacter>(Owner))
		{
			ItemDataComponent->InteractWith(Character);
		}
	}
	
	if (NearItem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NearItem"));
		
		UItemDataComponent* ItemDataComponent = NearItem->GetItemDataComponent();
		//ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());

		AActor* Owner = GetOwner();
		if (APlayerCharacter* Character = Cast<APlayerCharacter>(Owner))
		{
			ItemDataComponent->InteractWith(Character);
		}
	}

	return;
}

bool UInventoryComponent::Server_Interact_Validate()
{
	return true;
}




