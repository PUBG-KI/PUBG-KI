// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Armor/Armor_Base.h"

#include "Character/PlayerCharacter.h"
#include "Component/Inventory/InventoryComponent.h"

AArmor_Base::AArmor_Base()
{
	bReplicates = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	
	Name = NAME_None;

	Weight = -1.0f;
	Defense = -1.0f;
	Durability = -1.0f;
}

void AArmor_Base::BeginPlay()
{
	Super::BeginPlay();

	if (!Name.IsNone())
	{
		FString ArmorTablePath = "/Game/Datatables/Armor/DT_Armor.DT_Armor";
		UDataTable* ArmorTable = LoadObject<UDataTable>(nullptr, TEXT("ArmorTablePath"));
		FArmorStruct* FoundData = ArmorTable->FindRow<FArmorStruct>(Name, TEXT("Fail Find Row"));

		if (FoundData) // nullptr 체크 필수
		{
			ArmorData = *FoundData; // 구조체 값을 복사해서 대입
		}
	}

	if (Durability == -1)
	{
		Durability = ArmorData.Durability;
	}
	if (Defense == -1)
	{
		Defense = ArmorData.Defense;
	}
	if (Weight == -1)
	{
		Weight = ArmorData.Weight;
	}

	
}

void AArmor_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArmor_Base, StaticMeshComponent);

}

void AArmor_Base::OnRep_StaticMeshComponent()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_StaticMeshComponent"));
}

void AArmor_Base::EquipArmor(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->SetArmor(PlayerCharacter->GetArmor() + Defense);
	PlayerCharacter->GetInventoryComponent()->SetMaxInventoryWeight(PlayerCharacter->GetInventoryComponent()->GetMaxInventoryWeight() + Weight);
}

void AArmor_Base::UnEquipArmor(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->SetArmor(PlayerCharacter->GetArmor() - Defense);
	PlayerCharacter->GetInventoryComponent()->SetMaxInventoryWeight(PlayerCharacter->GetInventoryComponent()->GetMaxInventoryWeight() - Weight);
}
