// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/DataTable/DT_Weapon.h"
#include "EquipableItem.generated.h"

enum class EEquippedItemCategory : uint8;

UCLASS()
class PUBG_API AEquipableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipableItem();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	EEquippedItemCategory EquipSlot;

public:
	virtual EEquippedItemCategory GetEquipSlot() const { return EquipSlot; }
	void SetEquipSlot(EEquippedItemCategory OutEquippedItemCategory) { EquipSlot = OutEquippedItemCategory; } 

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
