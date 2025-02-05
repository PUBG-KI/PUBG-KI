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
	//SetIsReplicatedByDefault(true);
	SetIsReplicated(true);

	ItemID.DataTable = nullptr;
	ItemID.RowName = NAME_None;
	Quantity = 0;

	
}


// Called when the game starts
void UItemDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	//AActor* Owner = GetOwner(); 
	//if (Owner)
	{
	//	Owner->SetReplicates(true);
	}
}

void UItemDataComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemDataComponent, Quantity);

	//DOREPLIFETIME_CONDITION(UItemDataComponent, Quantity, COND_OwnerOnly);

}

void UItemDataComponent::OnRep_Quantity()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Quantity Replicate!"));

	
}

FText UItemDataComponent::LookAt()
{
	return FText::FromString("");
}

void UItemDataComponent::InteractWith_Implementation(APlayerCharacter* Character)
{
	//IInteractInterface::InteractWith_Implementation(Character);

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server ItemDataComponent::InteractWith_Implementation"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client ItemDataComponent::InteractWith_Implementation"));
	}


	
	if (UInventoryComponent* Inventory = Character->GetInventoryComponent())
	{
		// UE_LOG(LogTemp, Warning, TEXT("1"));
		UE_LOG(LogTemp, Warning, TEXT("%s, %d"), *ItemID.RowName.ToString(), Quantity);

		// 0이 아니면 인벤이 꽉 차서 아이템이 다 안들어간 것 
		int32 RemainItemQuantity = Inventory->AddToInventory(ItemID.RowName, Quantity, Weight);
		if (RemainItemQuantity != 0)
		{

			// 몽타주
			
			//UE_LOG(LogTemp, Warning, TEXT("Remain Item : %s, %d"), *ItemID.RowName.ToString(), Quantity);
			
			AActor* Owner = GetOwner(); 
			if (Owner)
			{
				if (GetOwner() && GetOwner()->HasAuthority())
				{
					UE_LOG(LogTemp, Warning, TEXT("Execute Server : RemainItemQuantity "));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Execute Client : RemainItemQuantity "));
				}
				
				if (AItemBase* ItemBase = Cast<AItemBase>(Owner))
				{
					UE_LOG(LogTemp, Warning, TEXT("RemainItemQuantity : %d"), RemainItemQuantity);
					
					Quantity = RemainItemQuantity; // 리플리케이트되는 시간이 안맞아서 밑에서 강제로 리플리케이트 시킴
					
					ItemBase->GetItemStruct().Quantity = RemainItemQuantity;

					UE_LOG(LogTemp, Warning, TEXT("Before Quantity : %d"), Quantity);

					UE_LOG(LogTemp, Warning, TEXT("After Quantity : %d"), Quantity);

					
					
					//Character->GetInventoryComponent()->ReplicateContent(Character->GetInventoryComponent()->GetContent());
					//Character->GetInventoryComponent()->ServerSetContents_Implementation(Character->GetInventoryComponent()->GetContent());
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






