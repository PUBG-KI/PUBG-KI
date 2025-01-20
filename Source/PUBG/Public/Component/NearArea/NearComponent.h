// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NearComponent.generated.h"

class AItemBase;
struct FItemStruct;
struct FItemSlotStruct;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUBG_API UNearComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNearComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Inventory", meta=(AllowPrivateAccess=true))
	TArray<AItemBase*> GroundItem;

public:	
	UFUNCTION(BlueprintCallable)
	void AddToGroundItem();
	UFUNCTION(BlueprintCallable)
	void UpdateInventory();
	UFUNCTION(BlueprintCallable)
	bool ShouldUpdate(AItemBase* ItemBase);

	UFUNCTION(BlueprintCallable)
	TArray<AItemBase*> GetGroundItem() { return GroundItem; }
	
};
