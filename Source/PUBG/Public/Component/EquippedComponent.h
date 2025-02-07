// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquippedComponent.generated.h"


class AItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUBG_API UEquippedComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquippedComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 무기 5칸, 방어구(헬멧, 가방, 조끼, 길리슈트) 4칸, 의상 (머리, 안경, 마스크, 셔츠, 겉옷, 한벌 옷, 바지, 신발) 8칸 총 17칸
	// 01 = 주무기, 2 = 보조무기 , 3 = 근접무기, 4 = 투척무기, 5 = 헬멧, 6 = 가방, 7 = 조끼, 8 = 길리, 9 ~ 16 의상
	TArray<AItemBase*> EquippedItems[16];

	void EquipItem(AItemBase* Item);
	void UnEquipItem(AItemBase* Item);

	
};
