// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ItemData/ItemDataComponent.h"

#include "Character/TestCharacter.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Controller/BasePlayerController.h"
#include "Item/ItemBase.h"
#include "Widgets/Inventory/InventoryWidget.h"

// Sets default values for this component's properties
UItemDataComponent::UItemDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ItemID.DataTable = nullptr;
	ItemID.RowName = NAME_None;
	Quantity = 0;

	this->SetIsReplicated(true);
	
}


// Called when the game starts
void UItemDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	AActor* Owner = GetOwner(); 
	if (Owner)
	{
		Owner->SetReplicates(true);
	}
}

FText UItemDataComponent::LookAt()
{
	return FText::FromString("");
}

void UItemDataComponent::InteractWith_Implementation(APlayerCharacter* Character)
{
	IInteractInterface::InteractWith_Implementation(Character);

	if (UInventoryComponent* Inventory = Character->GetInventoryComponent())
	{
		//UE_LOG(LogTemp, Warning, TEXT("1"));
		UE_LOG(LogTemp, Warning, TEXT("%s, %d"), *ItemID.RowName.ToString(), Quantity);

		// 0이 아니면 인벤이 꽉 차서 아이템이 다 안들어간 것 
		if (int32 RemainItem = Inventory->AddToInventory(ItemID.RowName, Quantity, Weight) != 0)
		{
			// 몽타주 
			//UE_LOG(LogTemp, Warning, TEXT("Add"));
			
			

			AActor* Owner = GetOwner(); 
			if (Owner)
			{
				if (AItemBase* ItemBase = Cast<AItemBase>(Owner))
				{
					ItemBase->GetItemStruct().Quantity = RemainItem;
				}
			}
		}
		// 0이라면 다 들어가서 바닥에 있는 아이템이 없어져야 함 
		else
		{			
			AActor* Owner = GetOwner(); 
			if (Owner)
			{
				Owner->Destroy();
			}
		}
	}
}






