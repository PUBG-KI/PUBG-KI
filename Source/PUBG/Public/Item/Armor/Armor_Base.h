// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLibrary/DataStruct/ArmorStruct.h"
#include "Item/EquipableItem.h"
#include "Armor_Base.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class PUBG_API AArmor_Base : public AEquipableItem
{
	GENERATED_BODY()

public:
	AArmor_Base();
	
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	FArmorStruct ArmorData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	float Durability; // 내구도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	float Defense; // 방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	float Weight;

public:
	// Getter functions
	UFUNCTION(BlueprintCallable, Category="Armor")
	FArmorStruct GetArmorData() const { return ArmorData; }
	UFUNCTION(BlueprintCallable, Category="Armor")
	FName GetArmorName() const { return Name; }
	UFUNCTION(BlueprintCallable, Category="Armor")
	float GetDurability() const { return Durability; }
	UFUNCTION(BlueprintCallable, Category="Armor")
	float GetDefense() const { return Defense; }
	UFUNCTION(BlueprintCallable, Category="Armor")
	float GetWeight() const { return Weight; }

	// Setter functions
	UFUNCTION(BlueprintCallable, Category="Armor")
	void SetArmorData(FArmorStruct NewArmorData) { ArmorData = NewArmorData; }
	UFUNCTION(BlueprintCallable, Category="Armor")
	void SetName(FName NewName) { Name = NewName; }
	UFUNCTION(BlueprintCallable, Category="Armor")
	void SetDurability(float NewDurability) { Durability = NewDurability; }
	UFUNCTION(BlueprintCallable, Category="Armor")
	void SetDefense(float NewDefense) { Defense = NewDefense; }
	UFUNCTION(BlueprintCallable, Category="Armor")
	void SetWeight(float NewWeight) { Weight = NewWeight; }
	UFUNCTION(BlueprintCallable, Category="Armor")
	void SetStaticMesh(UStaticMeshComponent* NewStaticMeshComponent) { StaticMeshComponent = NewStaticMeshComponent; }
	
	UFUNCTION(BlueprintCallable, Category="Armor")
	void EquipArmor(APlayerCharacter* PlayerCharacter);
	UFUNCTION(BlueprintCallable, Category="Armor")
	void UnEquipArmor(APlayerCharacter* PlayerCharacter);
	
};
