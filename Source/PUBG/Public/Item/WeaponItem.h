// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API AWeaponItem : public AItemBase
{
	GENERATED_BODY()

	AWeaponItem();

public:
	void InteractWith(APlayerCharacter* Character);
	virtual void InteractWith_Implementation(APlayerCharacter* Character) override;
};
