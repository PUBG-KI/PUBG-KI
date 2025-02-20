// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemEffect/BoosterEffect.h"

#include "BaseLibrary/DataStruct/BoosterEffectStruct.h"

ABoosterEffect::ABoosterEffect()
{
	BoosterEffectTableRowName = FName();

	BoosterEffectSkeletalMesh = CreateDefaultSubobject<USkeletalMesh>("SkeletalMesh");
}

void ABoosterEffect::BeginPlay()
{
	Super::BeginPlay();

	FString BoosterEffectTablePath = "/Game/Datatables/ItemEffect/BoosterEffect/DT_BoosterEffect.DT_BoosterEffect";
	BoosterEffectTable = LoadObject<UDataTable>(nullptr, TEXT("BoosterEffectTablePath"));

	if (!BoosterEffectTableRowName.IsNone())
	{
		FBoosterEffectStruct* Row = BoosterEffectTable->FindRow<FBoosterEffectStruct>(BoosterEffectTableRowName, TEXT("Find Row"));

		
		UseTime = Row->UseTime; // 사용 시간 지정
		StaminaChargeValue = Row->StaminaChargeValue; // 회복량 지정 
		// 애니메이션 에셋 지정
		// 몽타주 지정 
	}
}

void ABoosterEffect::UseItem()
{
	
}
 