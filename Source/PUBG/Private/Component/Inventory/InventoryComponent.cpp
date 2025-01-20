// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Inventory/InventoryComponent.h"

#include "AudioMixerBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "Character/TestCharacter.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "Interface/InteractInterface.h"
#include "Item/ItemBase.h"
//#include "Math/UnrealMathNeon.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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

	DOREPLIFETIME_CONDITION(UInventoryComponent, Content, COND_OwnerOnly);

}

int32 UInventoryComponent::AddToInventory(FName ItemID, int32 Quantity, int32 Weight)
{
	int32 RemainingQuantity = Quantity;
	
	UE_LOG(LogTemp, Warning, TEXT("CurrentInventoryWeight : %f"), CurrentInventoryWeight)
	;
	// 인벤이 가득 찼는지 검사, 현재 무게와 최대 무게 비교 false면 인벤에 넣을 수 있음 
	bool IsFull = CurrentInventoryWeight + Weight > MaxInventoryWeight;
	
	for (int32 i = RemainingQuantity; i>0 && !IsFull; i--, RemainingQuantity--)
	{
		IsFull = CurrentInventoryWeight >= MaxInventoryWeight;

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

	UE_LOG(LogTemp, Warning, TEXT("CurrentRemainingQuantity : %d"), RemainingQuantity);
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

int32 UInventoryComponent::AddToInventory1(FName ItemID, int32 Quantity)
{
	int32 RemainQuantity = Quantity;
	UE_LOG(LogTemp, Warning, TEXT("RemainQuantity: %d"), RemainQuantity);

	bool HasFailed = false;

	// 하나씩 인벤에 추가하는 구조
	for (int32 i = RemainQuantity; i > 0 && !HasFailed; i--)
	{
		UE_LOG(LogTemp, Warning, TEXT("i: %d"), i);

		// 인벤의 빈 칸을 찾음 
		int SlotIndex = FindSlot(ItemID);
		UE_LOG(LogTemp, Warning, TEXT("SlotIndex: %d"), SlotIndex);
		
		if (SlotIndex != -1)
		{
			AddToStack(SlotIndex, 1);
		}
		else
		{
			if (AnyEmptySlotsAvailable() != -1)
			{
				if (CreateNewStack(ItemID, 1))
				{
					
				}
				else
				{
					HasFailed = true;
				}
			}
		}
	}
	return RemainQuantity;
}
	
int32 UInventoryComponent::FindSlot(FName ItemID)
{
	for (int Index = 0; Index < Content.Num(); Index++)
	{
		FItemSlotStruct ItemSlot = Content[Index];
		
		if (ItemID == ItemSlot.ItemName)
		{
			if (ItemSlot.Quantity < GetMaxStackSize(ItemID))
			{
				return Index;
			}
		}
	}
	return -1;
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

void UInventoryComponent::AddToStack(int32 index, int32 Quantity)
{
	FItemSlotStruct ItemSlot;
	ItemSlot.ItemName = Content[index].ItemName;
	ItemSlot.Quantity = Content[index].Quantity + Quantity;

	Content[index] = ItemSlot;

	
}

int32 UInventoryComponent::AnyEmptySlotsAvailable()
{
	for (int32 Index = 0; Index < Content.Num(); Index++)
	{
		const FItemSlotStruct& ItemStruct = Content[Index];
		UE_LOG(LogTemp, Warning, TEXT("ItemSlot[%d]: %d"), Index, ItemStruct.Quantity);
		if (ItemStruct.Quantity == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Item at index %d has quantity 0"), Index);
			return Index;
		}
	}
	return -1;
}

bool UInventoryComponent::CreateNewStack(FName ItemID, int32 Quantity)
{
	int32 Index = AnyEmptySlotsAvailable();
	UE_LOG(LogTemp, Warning, TEXT("CreateNewStack : %d"), Index);
	if (Index != -1)
	{
		FGameplayTag Tag = FGameplayTag::EmptyTag;

		FItemSlotStruct ItemSlot;
		ItemSlot.ItemName = ItemID;
		ItemSlot.Quantity = Quantity;
		
		Content[Index] = ItemSlot;

		return true;
	}
	return false;
}

void UInventoryComponent::PrintContents()
{
	for (int32 Index = 0; Index < Content.Num(); ++Index)
	{
		FString Msg = Content[Index].ItemName.ToString();
		int32 Quantity = Content[Index].Quantity;
		
		UE_LOG(LogTemp, Warning, TEXT("%d, %s, %d"), Index, *Msg, Quantity);
	}
}

void UInventoryComponent::Server_Interact_Implementation()
{
	if (Item != nullptr)
	{
		UItemDataComponent* ItemDataComponent = Item->GetItemDataComponent();
		ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());

		AActor* Owner = GetOwner();
		if (APlayerCharacter* Character = Cast<APlayerCharacter>(Owner))
		ItemDataComponent->InteractWith(Character);
	}
}

bool UInventoryComponent::Server_Interact_Validate()
{
	return true;
}


