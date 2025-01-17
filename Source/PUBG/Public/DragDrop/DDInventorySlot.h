// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DDInventorySlot.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class PUBG_API UDDInventorySlot : public UDragDropOperation
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UInventoryComponent* InventoryComponent;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 ContentIndex;


public:
	UFUNCTION(BlueprintCallable)
	void SetInventoryComponent(UInventoryComponent* OutInventoryComponent) { InventoryComponent = OutInventoryComponent; }
	UFUNCTION(BlueprintCallable)
	void SetContentIndex(int32 InContentIndex) { ContentIndex = InContentIndex; }

	UFUNCTION(BlueprintCallable)
	void UpdateDDInventorySlot();
};
