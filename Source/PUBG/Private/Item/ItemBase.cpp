// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBase.h"

//#include "K2Node_GetDataTableRow.h"
#include "Camera/CameraComponent.h"
#include "Character/TestCharacter.h"
#include "Components/BoxComponent.h"
#include "Character/TestCharacter.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/PlayerCharacter.h"


//int32 AItemBase::BeginOverlapCount = 0;

AItemBase::AItemBase()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(StaticMesh);
	BoxComponent->SetupAttachment(StaticMesh);
	InteractionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetupAttachment(StaticMesh);

	
	InteractionComponent->InitBoxExtent(FVector(20.0f));

	ItemDataComponent = CreateDefaultSubobject<UItemDataComponent>(TEXT("ItemDataComponent"));

	//BeginOverlapCount = 0;
	bReplicates = true;
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();


	
	//BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnComponentBeginOverlap);
	//BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnComponentEndOverlap);
	
}

// void AItemBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	if (ATestCharacter* TestCharacter = Cast<ATestCharacter>(OtherActor))
// 	{
// 		BeginOverlapCount += 1;
// 		UE_LOG(LogTemp, Warning, TEXT("%d"), BeginOverlapCount);
//
// 		//UE_LOG(LogTemp, Warning, TEXT("Test Character"));
// 		FTimerDelegate TimerDelegate;
// 		TimerDelegate.BindLambda([OtherActor]()
// 		{
// 			if (ATestCharacter* TestCharacter = Cast<ATestCharacter>(OtherActor))
// 			{
// 				UCameraComponent* Camera = TestCharacter->GetFollowCamera();
//
// 				FHitResult Hit;
// 				FVector Start = Camera->K2_GetComponentLocation();
// 				FVector End = Start + UKismetMathLibrary::GetForwardVector(Camera->K2_GetComponentRotation()) * 380.0f;
// 				TArray<AActor*> ActorsToIgnore;
// 				ActorsToIgnore.Add(TestCharacter);
// 				ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
// 				//TraceType = ETraceTypeQuery
//
// 				if (UInventoryComponent* CharacterInventory = TestCharacter->GetInventoryComponent())
// 				{
// 					//if (CharacterInventory->GetItem() == nullptr)
// 					{
// 						UKismetSystemLibrary::LineTraceSingle(TestCharacter->GetWorld(), Start, End, TraceType, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, false, FLinearColor(1, 0, 0, 0), FLinearColor (0, 1, 0, 1));
//
// 						if (AItemBase* Item = Cast<AItemBase>(Hit.GetActor()))
// 						{
// 							//UE_LOG(LogTemp, Warning, TEXT("Item Base"));
//
// 							if (Item != CharacterInventory->GetItem())
// 							{
// 								CharacterInventory->SetItem(Item);
// 								if (Item->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
// 								{
// 									//UE_LOG(LogTemp, Warning, TEXT("Item %s"), *Item->GetName());
//
// 									Item->LookAt();
// 								}
// 							}
// 						}
// 						else
// 						{
// 							CharacterInventory->SetItem(nullptr);
// 						}
// 					}
// 				}
// 				
// 				
// 			}
// 		});
//
// 		GetWorldTimerManager().SetTimer(BeginOverlapTimerHandle, TimerDelegate, 0.1f, true);
// 		
// 	}
// }
//
// void AItemBase::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	BeginOverlapCount -= 1;
// 	UE_LOG(LogTemp, Warning, TEXT("%d"), BeginOverlapCount);
//
// 	if (BeginOverlapCount < 0)
// 	{
// 		BeginOverlapCount = 0;
// 	}
// 	
// 	if (BeginOverlapCount == 0)
// 	{
// 		GetWorldTimerManager().ClearTimer(BeginOverlapTimerHandle);
//
// 		if (ATestCharacter* TestCharacter = Cast<ATestCharacter>(OtherActor))
// 		{
// 			UInventoryComponent* CharacterInventory = TestCharacter->GetInventoryComponent();
// 			CharacterInventory->SetItem(nullptr);
// 		}
// 	}
// 	
// }

FText AItemBase::LookAt()
{
	checkf(ItemDataComponent, TEXT("ItemDataComponent is Null"));
	FName Name = ItemDataComponent->GetItemRowName();
	FText Message = FText::FromString("Picked up the " + Name.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Message.ToString());
	
	//UK2Node_GetDataTableRow()
	ItemOfZ = this->GetActorLocation().Z;
	UE_LOG(LogTemp, Warning, TEXT("ItemofZ : %f"), ItemOfZ);
	
	return Message;
}

void AItemBase::InteractWith_Implementation(APlayerCharacter* Character)
{
	IInteractInterface::InteractWith_Implementation(Character);
	
	//UE_LOG(LogTemp, Display, TEXT("2"));
	//UE_LOG(LogTemp, Display, TEXT("Interacting with %s"), *GetName());

	UInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
	InventoryComponent->SetItem(this);
	// ItemOfZ = this->GetActorLocation().Z;
	
	if (InventoryComponent->GetItem() != nullptr)
	{
		InventoryComponent->Server_Interact();
		
		 if (this->ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		 {
		 	
		 	ItemDataComponent->InteractWith(Character);
		 }
	}
	
	//Destroy();
}

void AItemBase::SetMesh(UStaticMesh* NewMesh)
{
	StaticMesh->SetStaticMesh(NewMesh);
}




