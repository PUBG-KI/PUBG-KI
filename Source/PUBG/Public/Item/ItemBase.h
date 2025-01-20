// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/InteractInterface.h"
#include "Item/BaseItem.h"
#include "ItemBase.generated.h"

class UItemDataComponent;
class UBoxComponent;
/**
 * 
 */


UCLASS()
class PUBG_API AItemBase : public ABaseItem, public IInteractInterface
{
	GENERATED_BODY()


protected:
	AItemBase();
	
	virtual void BeginPlay() override;

// 변수 부분 
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FItemStruct ItemStruct;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InteractionComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UItemDataComponent* ItemDataComponent;
	
	FTimerHandle BeginOverlapTimerHandle;
	
	//static int32 BeginOverlapCount;

// 함수 부분
private:
	// UFUNCTION()
	// void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// UFUNCTION()
	// void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);	

public:
	virtual FText LookAt() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractWith(APlayerCharacter* Character);
	virtual void InteractWith_Implementation(APlayerCharacter* Character) override;

	//Getter
	UFUNCTION()
	UItemDataComponent* GetItemDataComponent() const {return ItemDataComponent; }
	UBoxComponent* GetBoxComponent() const { return BoxComponent; }
	FItemStruct& GetItemStruct() { return Item; }
	
	void SetItem(FItemStruct const &OutItem) { Item = OutItem; }
	
};
