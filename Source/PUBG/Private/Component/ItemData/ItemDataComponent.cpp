// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ItemData/ItemDataComponent.h"

#include "Character/TestCharacter.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Components/BoxComponent.h"
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

void UItemDataComponent::InteractWith_Implementation(ATestCharacter* Character)
{
	IInteractInterface::InteractWith_Implementation(Character);

	if (UInventoryComponent* Inventory = Character->GetInventoryComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s, %d"), *ItemID.RowName.ToString(), Quantity);
		if (Inventory->AddToInventory(ItemID.RowName, Quantity, Weight) != 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Add"));

			// for (int i = 0 ; i < Inventory->GetContent().Num(); i++)
			// {
			// 	Character->GetInventoryWidget()->AddNewChild();
			// }
			
			Character->GetInventoryWidget()->UpdateInventoryWidget();
			
			AActor* Owner = GetOwner(); 
			if (Owner)
			{
				Owner->Destroy();
			}
		}
	}
}






