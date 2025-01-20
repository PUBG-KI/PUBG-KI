// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/NearArea/NearComponent.h"

#include "BaseLibrary/DataStruct/ItemSlotStruct.h"
#include "BaseLibrary/DataStruct/ItemStruct.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "Item/ItemBase.h"

// Sets default values for this component's properties
UNearComponent::UNearComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UNearComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UNearComponent::AddToGroundItem()
{
	
}

void UNearComponent::UpdateInventory()
{
	GroundItem.Sort([](const AItemBase& LHS, const AItemBase& RHS)
	{
			FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");
			UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));
	
			if (DataTable)
			{
				UE_LOG(LogTemp, Warning, TEXT("DataTable!!"));
			}
		
			FItemStruct* LHSRow = DataTable->FindRow<FItemStruct>(LHS.GetItemDataComponent()->GetItemRowName(), TEXT("Get Item Row"));
			FItemStruct* RHSRow = DataTable->FindRow<FItemStruct>(RHS.GetItemDataComponent()->GetItemRowName(), TEXT("Get Item Row"));
			
			UE_LOG(LogTemp, Warning, TEXT("LHS.ItemName: %s, RHS.ItemName: %s"), *LHSRow->Name.ToString(), *RHSRow->Name.ToString());
			
			//LHSRow 또는 RHSRow가 nullptr인 경우를 처리
		   if (!LHSRow || !RHSRow)
		   {
			   UE_LOG(LogTemp, Warning, TEXT("Row not found for one or both items!"));
			   return false;
		   }
			
			if (LHSRow->Category != RHSRow->Category)
			{
				return LHSRow->Category < RHSRow->Category;
			}
				
			if (LHSRow->Name != RHSRow->Name)
			{
				return LHSRow->Name.ToString() < RHSRow->Name.ToString();  // Name이 다르면 Name 순으로 오름차순 정렬
			}
				
			return LHSRow->Quantity > RHSRow->Quantity;  // Quantity가 다르면 Quantity 순으로 오름차순 정렬
	});
}

bool UNearComponent::ShouldUpdate(AItemBase* ItemBase)
{
	for (int i = 0; i> GroundItem.Num(); i++)
	{
		if (GroundItem[i] != ItemBase)
		{
			return false;
			
		}
	}
	return true;
}





