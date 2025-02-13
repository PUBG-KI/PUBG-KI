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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


// 변수 부분 
private:
	UPROPERTY(Replicated, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FItemStruct ItemStruct;
	UPROPERTY(Replicated, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FItemStruct Item;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InteractionComponent;
	UPROPERTY(ReplicatedUsing = OnRep_ItemDataComponent, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
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
	// OnRep
	UFUNCTION(BlueprintCallable)
	void OnRep_ItemDataComponent();
	//Getter
	UFUNCTION()
	UItemDataComponent* GetItemDataComponent() const {return ItemDataComponent; }
	UBoxComponent* GetBoxComponent() const { return BoxComponent; }
	FItemStruct& GetItemStruct() { return Item; }

	// Setter
	UFUNCTION(BlueprintCallable)
	void SetItem(FItemStruct const &OutItem) { Item = OutItem; }
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetItem(FItemStruct const &OutItem);
	UFUNCTION(BlueprintCallable)
	void SetItemStruct(FItemStruct OutItemStruct) { ItemStruct = OutItemStruct; }
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetItemStruct(FItemStruct OutItemStruct);
	
	virtual FText LookAt() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractWith(APlayerCharacter* Character);
	virtual void InteractWith_Implementation(APlayerCharacter* Character) override;


	UFUNCTION()
	void SetMesh(UStaticMesh* NewMesh);
	

private:
	UPROPERTY()
	float ItemOfZ;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 TableIndex;
public:
	
	FORCEINLINE void SetItemOfZ(float InItemOfZ) { ItemOfZ = InItemOfZ; }
	FORCEINLINE float GetItemOfZ() const { return ItemOfZ; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetTableIndex(int32 OutTableIndex) { TableIndex = OutTableIndex; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetTableIndex() const { return TableIndex; }
	
};
