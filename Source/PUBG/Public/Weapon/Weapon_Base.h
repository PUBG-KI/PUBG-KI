// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseLibrary/BaseStructType.h"
#include "GameplayAbilitySpecHandle.h"
#include "DataTable/DT_Weapon.h"
#include "Item/EquipableItem.h"
#include "Weapon_Base.generated.h"

UCLASS()
class PUBG_API AWeapon_Base : public AEquipableItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponSkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FPlayerWeaponData PlayerWeaponData;
	
	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;
	
	UPROPERTY(EditDefaultsOnly)
	FWeaponData WeaponDataAsset;


private:

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;


public:
	UFUNCTION(BlueprintCallable, Category = "DataAsset")
	FWeaponData GetWeaponDataAsset() const {return WeaponDataAsset;}

	UFUNCTION(BlueprintCallable, Category = "DataAsset")
	void SetWeaponDataAsset(FWeaponData WeaponData);
	
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	
	
};


