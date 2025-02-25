// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Equipped/EquippedComponent.h"

#include "BaseLibrary/DataEnum/ItemEnum.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Controller/BasePlayerController.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Item/ArmorItem.h"
#include "Item/ItemBase.h"
#include "Item/WeaponItem.h"
#include "Item/Armor/Armor_Base.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Inventory/InventoryWidget.h"

// Sets default values for this component's properties
UEquippedComponent::UEquippedComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	SetIsReplicatedByDefault(true);
	
	
}

void UEquippedComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquippedComponent, CurrentWeapon);
	DOREPLIFETIME(UEquippedComponent, LastCurrentWeapon);
	DOREPLIFETIME(UEquippedComponent, PrimarySlot);
	DOREPLIFETIME(UEquippedComponent, SecondarySlot);
	
	DOREPLIFETIME(UEquippedComponent, EquippedItems);
}


// Called when the game starts
void UEquippedComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");	
	ItemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));

	//EquippedMainWeapon.SetNum(2);
	EquippedItems.SetNum(16);
}


// Called every frame
void UEquippedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquippedComponent::OnRep_EquippedItems()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_EquippedItems!!"));

	UTextureRenderTarget2D* LoadedRTSlot1 = nullptr;
	UTextureRenderTarget2D* LoadedRTSlot2 = nullptr;
	UTextureRenderTarget2D* LoadedRTSlot3 = nullptr;

	for (int i = 0; i < EquippedItems.Num(); i++)
	{
		if (EquippedItems[i] != nullptr)
		{
			switch (i)
			{
			case 0:
				LoadedRTSlot1 = Cast<UTextureRenderTarget2D>(StaticLoadObject(
				UTextureRenderTarget2D::StaticClass(), 
				nullptr, 
				TEXT("/Game/Blueprint/Widgets/Materials/RenderTarget/RT_1Slot.RT_1Slot")
				));

				EquippingWeaponUpdate(EquippedItems[i], LoadedRTSlot1);
				// EquippedItems[i]->GetSceneCaptureComponent()->TextureTarget = LoadedRTSlot1;
				// EquippedItems[i]->GetSceneCaptureComponent()->ShowOnlyActorComponents(EquippedItems[i], true);
				// EquippedItems[i]->GetSceneCaptureComponent()->bCaptureEveryFrame = false;
				// EquippedItems[i]->GetSceneCaptureComponent()->bCaptureOnMovement = false;
				//
				// EquippedItems[i]->GetSceneCaptureComponent()->CaptureScene();
				break;
			case 1:
				LoadedRTSlot2 = Cast<UTextureRenderTarget2D>(StaticLoadObject(
				UTextureRenderTarget2D::StaticClass(), 
				nullptr, 
				TEXT("/Game/Blueprint/Widgets/Materials/RenderTarget/2Slot/RT_2Slot.RT_2Slot")
				));

				EquippingWeaponUpdate(EquippedItems[i], LoadedRTSlot2);
				// EquippedItems[i]->GetSceneCaptureComponent()->TextureTarget = LoadedRTSlot2;
				// EquippedItems[i]->GetSceneCaptureComponent()->ShowOnlyActorComponents(EquippedItems[i], true);
				// EquippedItems[i]->GetSceneCaptureComponent()->bCaptureEveryFrame = false;
				// EquippedItems[i]->GetSceneCaptureComponent()->bCaptureOnMovement = false;
				//
				// EquippedItems[i]->GetSceneCaptureComponent()->CaptureScene();
				break;
			case 2:
				LoadedRTSlot3 = Cast<UTextureRenderTarget2D>(StaticLoadObject(
				UTextureRenderTarget2D::StaticClass(), 
				nullptr, 
				TEXT("/Game/Blueprint/Widgets/Materials/RenderTarget/3Slot/RT_3Slot.RT_3Slot")
				));

				EquippingWeaponUpdate(EquippedItems[i], LoadedRTSlot3);


				// EquippedItems[i]->GetSceneCaptureComponent()->TextureTarget = LoadedRTSlot3;
				// EquippedItems[i]->GetSceneCaptureComponent()->ShowOnlyActorComponents(EquippedItems[i], true);
				// EquippedItems[i]->GetSceneCaptureComponent()->bCaptureEveryFrame = false;
				// EquippedItems[i]->GetSceneCaptureComponent()->CaptureScene();
				break;
			}
			
			
		}
	}
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
				PlayerController->GetInventoryWidget()->UpdateEquippedWidget();
				UE_LOG(LogTemp, Warning, TEXT("OnRep_EquippedItems : Widget Update!"));
			}
		}
	}
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
		
	AEquipableItem* EquipableItem = GetWorld()->SpawnActor<AEquipableItem>(Row->BP_Item.Get());
	

	
	

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
	UE_LOG(LogTemp, Warning, TEXT("ServerEquipMainItem_Implementation : ItemId %s"), *ItemID.ToString());

	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));

	int32 MainWeaponSlot = static_cast<int32>(WeaponItem->GetEquippedItemCategory());
	
	TSubclassOf<AGun_Base> GunClass = Cast<UClass>(Row->BP_Item.Get());
	
	
	if (AGun_Base* MainWeapon = GetWorld()->SpawnActorDeferred<AGun_Base>(GunClass, FTransform(FRotator(0), FVector(0))))
	{
		//TempWeapon->SetTableIndex(RowIndex);
		FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
		

		int32 RemainSlot = FindSlotMainWeapon();
		UE_LOG(LogTemp, Warning, TEXT("RemainSlot : %d"), RemainSlot);

		EEquippedItemCategory EquippedItemCategory = static_cast<EEquippedItemCategory>(MainWeaponSlot);
		MainWeapon->SetEquipSlot(EquippedItemCategory);
		MainWeaponSlot = static_cast<int32>(MainWeapon->GetEquipSlot()); // 장착될 슬롯값 가져오기
	
		if (RemainSlot == -1) // Main 무기 슬롯이 가득찬 것 
		{
			int32 DropMainWeaponSlot = DropMainWeapon();
			RemainSlot = DropMainWeaponSlot;
			UE_LOG(LogTemp, Warning, TEXT("DropMainWeapon RemainSlot : %d"), RemainSlot);
		}
	
		if (RemainSlot == 0) // 1번 슬롯이 비어있는 것 
		{
			// 필요한 값 넣기 MainWeapon -> Weapon
			
			EquippedItems[MainWeaponSlot] = MainWeapon;
			EquippedItems[MainWeaponSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_primarySocket")));
			UE_LOG(LogTemp, Warning, TEXT(" Test Debug!!!!!!!!!!!!!!!!!!!!!!!!!!!!"))

		}
		else if (RemainSlot == 1) // 2번 슬롯이 비어있는 것 
		{
			MainWeapon->SetEquipSlot(EEquippedItemCategory::SecondarySlot);
			MainWeaponSlot = static_cast<int32>(MainWeapon->GetEquipSlot());
			
			EquippedItems[MainWeaponSlot] = MainWeapon;
			EquippedItems[MainWeaponSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_secondarySocket")));
		}
		
		MainWeapon->FinishSpawning(FTransform(FRotator(0), FVector(0)));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerEquipMainItem_Implementation : MainWeapon Casting Fail"));
	}
	GetOwner()->ForceNetUpdate();
	Item->Destroy(true);
	
	// AGun_Base* MainWeapon = Cast<AGun_Base>(GetWorld()->SpawnActor<AEquipableItem>(Row->BP_Item));
	// FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	//
	// APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	//
	// int32 RemainSlot = FindSlotMainWeapon();
	//
	// int32 MainWeaponSlot = static_cast<int32>(MainWeapon->GetEquipSlot()); // 장착될 슬롯값 가져오기
	//
	// if (RemainSlot == -1) // Main 무기 슬롯이 가득찬 것 
	// {
	// 	int32 DropMainWeaponSlot = DropMainWeapon();
	// 	RemainSlot = DropMainWeaponSlot;
	// }
	//
	// if (RemainSlot == 0) // 1번 슬롯이 비어있는 것 
	// {
	// 	EquippedItems[MainWeaponSlot] = MainWeapon;
	// 	EquippedItems[MainWeaponSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_primarySocket")));
	//
	// 	// 필요한 값 넣기 MainWeapon -> Weapon
	// }
	// else if (RemainSlot == 1) // 2번 슬롯이 비어있는 것 
	// {
	// 	MainWeapon->SetEquipSlot(EEquippedItemCategory::SecondarySlot);
	// 	MainWeaponSlot = static_cast<int32>(MainWeapon->GetEquipSlot());
	// 	
	// 	
	// 	EquippedItems[MainWeaponSlot] = MainWeapon;
	// 	EquippedItems[MainWeaponSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_secondarySocket")));
	// }

}

int32 UEquippedComponent::DropMainWeapon(AGun_Base* OutCurrentWeapon)
{
	// 버리는 것은 무조건 UI에서만, 지정해서
	// 교체할 때 쓰기 위해서는 버릴 아이템을 매개변수로 
	
	if (OutCurrentWeapon == nullptr) // 매개변수가 없다는 것은 교체할 때 쓰는 것 CurrentWeapon과 2번 슬롯의 무기 
	{
		
		if (CurrentWeapon != nullptr && (CurrentWeapon->GetEquipSlot() == EEquippedItemCategory::PrimarySlot || CurrentWeapon->GetEquipSlot() == EEquippedItemCategory::SecondarySlot)) // 현재 손에 든 무기가 있으면 현재 무기와 교체
		{
			ServerSpawnStaticMeshFromMainWeapon(Cast<AGun_Base>(CurrentWeapon));
			
			int32 DropIndex = static_cast<int32>(CurrentWeapon->GetEquipSlot());
			UE_LOG(LogTemp, Warning, TEXT("Drop Index : %d"), DropIndex);

			CurrentWeapon->Destroy();
			// 능력과 레이어 뺴는 곳 
			CurrentWeapon = nullptr;
			return DropIndex;
		}
		else // 현재 손에 든 무기가 없으면 2번 슬롯 무기와 교체
		{
			UE_LOG(LogTemp, Warning, TEXT("Slot : 2"));
			AGun_Base* MainWeapon = Cast<AGun_Base>(EquippedItems[1]);
			ServerSpawnStaticMeshFromMainWeapon(MainWeapon);
			
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
		ServerSpawnStaticMeshFromMainWeapon(Cast<AGun_Base>(OutCurrentWeapon));

		int32 DropIndex = static_cast<int32>(OutCurrentWeapon->GetEquipSlot());
		OutCurrentWeapon = nullptr;
		return DropIndex;
	}
}

void UEquippedComponent::ServerSpawnStaticMeshFromMainWeapon_Implementation(AGun_Base* OutCurrentWeapon)
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
			TempWeapon->SetItemTableRowName(ItemID);
			//TempWeapon->SetTableIndex(-1); // 무기 버릴 때 자기 데이터를 넣어줘야 함 
			TempWeapon->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
			UE_LOG(LogTemp, Warning, TEXT("TempWeapon GetItemRowName : %s"), *TempWeapon->GetItemDataComponent()->GetItemRowName().ToString());

		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load blueprint class! Check the path: /Game/Blueprint/Item/Farming/TestWeaponItem.TestWeaponItem_C"));
	}
}


void UEquippedComponent::ServerEquipSubWeapon_Implementation(AItemBase* Item)
{
	AWeaponItem* SubWeaponItem = Cast<AWeaponItem>(Item);

	FName ItemID = SubWeaponItem->GetItemDataComponent()->GetItemRowName();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemID.ToString());
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
	
	int32 SubWeaponSlot = static_cast<int32>(SubWeaponItem->GetEquippedItemCategory()); // Item에 저장되어 있는 슬롯 값 가져오기 

	if (EquippedItems[2] != nullptr)
	{
		DropSUbWeapon();
	}

	TSubclassOf<AGun_Base> GunClass = Cast<UClass>(Row->BP_Item.Get());
	if (AGun_Base* SubWeapon = GetWorld()->SpawnActorDeferred<AGun_Base>(GunClass, FTransform(FRotator(0), FVector(0))))
	{
		//TempWeapon->SetTableIndex(RowIndex);
		FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

		EEquippedItemCategory EquippedItemCategory = static_cast<EEquippedItemCategory>(SubWeaponSlot);
		SubWeapon->SetEquipSlot(EquippedItemCategory);
		
		SubWeaponSlot = static_cast<int32>(SubWeapon->GetEquipSlot()); // 장착될 슬롯값 가져오기
		UE_LOG(LogTemp, Warning, TEXT("SubWeapon Slot : %d"), SubWeaponSlot);

		EquippedItems[SubWeaponSlot] = SubWeapon;
		EquippedItems[SubWeaponSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("SideArm")));
		
		SubWeapon->FinishSpawning(FTransform(FRotator(0), FVector(0)));
	}

	GetOwner()->ForceNetUpdate();
	Item->Destroy(true);
}

void UEquippedComponent::DropSUbWeapon(AGun_Base* OutCurrentWeapon)
{
	if (OutCurrentWeapon != nullptr) // UI에서 버리는 것
	{
		// 권총은 하나이므로 권총 슬롯과 바로 비교 
		ServerSpawnStaticMeshFromSubWeapon(Cast<AGun_Base>(OutCurrentWeapon));
		OutCurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
	else // 교체되는 것 
	{
		if (CurrentWeapon != nullptr && CurrentWeapon->GetEquipSlot() == EEquippedItemCategory::SubWeapon) // 현재 손에 권총이 있으면 
		{
			ServerSpawnStaticMeshFromSubWeapon(Cast<AGun_Base>(CurrentWeapon));
			CurrentWeapon->Destroy();
			// 능력과 레이어 뺴는 곳 
			CurrentWeapon = nullptr;
		}
		else if (EquippedItems[2] != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("EquippedItems[2]"));
			ServerSpawnStaticMeshFromSubWeapon(Cast<AGun_Base>(EquippedItems[2]));
			EquippedItems[2]->Destroy();
			EquippedItems[2] = nullptr;
		}
	}
}

void UEquippedComponent::ServerSpawnStaticMeshFromSubWeapon_Implementation(AGun_Base* OutCurrentWeapon)
{
	FName ItemID = FName(OutCurrentWeapon->GetWeaponDataAsset().GunName); // RowName 가져오기
	UE_LOG(LogTemp, Warning, TEXT("ITemID : %s"), *ItemID.ToString());
	
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
			//TempWeapon->SetTableIndex(-1); // 무기 버릴 때 자기 데이터를 넣어줘야 함
			TempWeapon->SetItemTableRowName(ItemID);
			TempWeapon->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
			UE_LOG(LogTemp, Warning, TEXT("TempWeapon GetItemRowName : %s"), *TempWeapon->GetItemDataComponent()->GetItemRowName().ToString());

		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load blueprint class! Check the path: /Game/Blueprint/Item/Farming/TestWeaponItem.TestWeaponItem_C"));
	}
}


void UEquippedComponent::ServerEquiptHelmet_Implementation(AItemBase* Item)
{
	AArmorItem* HelmetItem = Cast<AArmorItem>(Item);

	FName ItemID = HelmetItem->GetItemDataComponent()->GetItemRowName();
	UE_LOG(LogTemp, Warning, TEXT("ServerEquiptHelmet_Implementation : %s"), *ItemID.ToString());
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
	
	int32 HelmetSlot = static_cast<int32>(HelmetItem->GetEquippedItemCategory()); // Item에 저장되어 있는 슬롯 값 가져오기 

	if (EquippedItems[HelmetSlot] != nullptr)
	{
		DropArmor(HelmetSlot);
	}

	TSubclassOf<AArmor_Base> ArmorClass = Cast<UClass>(Row->BP_Item.Get());
	if (AArmor_Base* Armor = GetWorld()->SpawnActorDeferred<AArmor_Base>(ArmorClass, FTransform(FRotator(0), FVector(0))))
	{
		//TempWeapon->SetTableIndex(RowIndex);
		FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

		EEquippedItemCategory EquippedItemCategory = static_cast<EEquippedItemCategory>(HelmetSlot);
		Armor->SetEquipSlot(EquippedItemCategory);
		Armor->GetStaticMeshComponent()->SetStaticMesh(Row->StaticMesh);
		
		HelmetSlot = static_cast<int32>(Armor->GetEquipSlot()); // 장착될 슬롯값 가져오기
		UE_LOG(LogTemp, Warning, TEXT("Helmet Slot : %d"), HelmetSlot);

		EquippedItems[HelmetSlot] = Armor;
		if (HelmetSlot == 5)
		{
			EquippedItems[HelmetSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("HeadSocket")));
		}
		else if (HelmetSlot == 6)
		{
			UE_LOG(LogTemp, Warning, TEXT("HelmetSlot == 6"));
			EquippedItems[HelmetSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("BagSocket")));
		}
		else if (HelmetSlot == 7)
		{
			EquippedItems[HelmetSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("ArmorSocket")));
		}
		
		Armor->FinishSpawning(FTransform(FRotator(0), FVector(0)));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Armor SpawnActorDeferred Failed"));
	}

	GetOwner()->ForceNetUpdate();
}

void UEquippedComponent::DropArmor(int32 OutIndex)
{
	if (EquippedItems[OutIndex] != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::DropArmor = EquippedItems[OutIndex]"));
		ServerSpawnStaticMeshFromArmor(Cast<AArmor_Base>(EquippedItems[OutIndex]));
		EquippedItems[OutIndex]->Destroy();
		EquippedItems[OutIndex] = nullptr;
	}
}

void UEquippedComponent::ServerSpawnStaticMeshFromArmor_Implementation(AArmor_Base* OutCurrentArmor)
{
	FName ItemID = FName(OutCurrentArmor->GetArmorData().Name); // RowName 가져오기
	UE_LOG(LogTemp, Warning, TEXT("ITemID : %s"), *ItemID.ToString());
	
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row")); // 테이블 가져오기
	int32 RowIndex = GetRowIndex(ItemDataTable, ItemID); // RowName에 해당하는 인덱스 가져오기 
	UE_LOG(LogTemp, Warning, TEXT("Row Index : %d"), RowIndex);

			
	FRotator SpawnRotation = FRotator(0, 0, 0);
	FVector SpawnLocation = DropLocation();
	UE_LOG(LogTemp, Warning, TEXT("SpawnLocation : %f %f %f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);

	UClass* ArmorItemBPClass = LoadClass<AWeaponItem>(nullptr, TEXT("/Game/Blueprint/Item/Farming/ArmorItem.ArmorItem_C"));
	if (ArmorItemBPClass) // nullptr 체크 추가
	{
		if (AArmorItem* TempArmor = GetWorld()->SpawnActorDeferred<AArmorItem>(ArmorItemBPClass, FTransform(SpawnRotation, SpawnLocation)))
		{
			//TempWeapon->SetTableIndex(RowIndex);
			//TempWeapon->SetTableIndex(-1); // 무기 버릴 때 자기 데이터를 넣어줘야 함
			
			TempArmor->SetItemTableRowName(ItemID);
			TempArmor->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
			UE_LOG(LogTemp, Warning, TEXT("TempWeapon GetItemRowName : %s"), *TempArmor->GetItemDataComponent()->GetItemRowName().ToString());

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
	//TArray<FName> RowNames = DataTable->FindRow<>();
	


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

	
	//return FVector(0, 0, 0);
}

void UEquippedComponent::PrintEquippedItems()
{
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : PrintEquippedItems"));

		for (int i = 0; i < EquippedItems.Num(); i++)
		{
			if (EquippedItems[i] != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("%d : %s"), i, *EquippedItems[i]->GetActorNameOrLabel());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : PrintEquippedItems"));

		for (int i = 0; i < EquippedItems.Num(); i++)
		{
			if (EquippedItems[i] != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("%d : %s"), i, *EquippedItems[i]->GetActorNameOrLabel());
			
			}
		}
		
	}
}

void UEquippedComponent::EquippingWeaponUpdate(AEquipableItem* OutEquippedItem, UTextureRenderTarget2D* OutLoadedRT)
{
	OutEquippedItem->GetSceneCaptureComponent()->TextureTarget = OutLoadedRT;
	OutEquippedItem->GetSceneCaptureComponent()->bCaptureEveryFrame = false;
	OutEquippedItem->GetSceneCaptureComponent()->bCaptureOnMovement = false;
	OutEquippedItem->GetSceneCaptureComponent()->bEnableClipPlane = false;
	OutEquippedItem->GetSceneCaptureComponent()->bConsiderUnrenderedOpaquePixelAsFullyTranslucent = false;
	OutEquippedItem->GetSceneCaptureComponent()->ShowOnlyActorComponents(OutEquippedItem, true);

	OutEquippedItem->GetSceneCaptureComponent()->PostProcessSettings.bOverride_ToneCurveAmount = true;
	OutEquippedItem->GetSceneCaptureComponent()->PostProcessSettings.ToneCurveAmount = 0.0f;
	OutEquippedItem->GetSceneCaptureComponent()->PostProcessSettings.bOverride_BloomIntensity = true;
	OutEquippedItem->GetSceneCaptureComponent()->PostProcessSettings.BloomIntensity = 0.0f;
	OutEquippedItem->GetSceneCaptureComponent()->PostProcessSettings.bOverride_AmbientOcclusionIntensity = true;
	OutEquippedItem->GetSceneCaptureComponent()->PostProcessSettings.AmbientOcclusionIntensity = 0.0f;
	OutEquippedItem->GetSceneCaptureComponent()->PostProcessBlendWeight = 1.0f;
	OutEquippedItem->GetSceneCaptureComponent()->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	OutEquippedItem->GetSceneCaptureComponent()->ProjectionType = ECameraProjectionMode::Orthographic;
	OutEquippedItem->GetSceneCaptureComponent()->OrthoWidth = 110.0f;

	

	OutEquippedItem->GetSceneCaptureComponent()->ShowFlags.SetDynamicShadows(false);
	OutEquippedItem->GetSceneCaptureComponent()->ShowFlags.SetStaticMeshes(true);
	OutEquippedItem->GetSceneCaptureComponent()->ShowFlags.SetSkeletalMeshes(true);
	
	OutEquippedItem->GetSceneCaptureComponent()->CaptureScene();

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






