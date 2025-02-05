// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/InteractInterface.h"
#include "ItemDataComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUBG_API UItemDataComponent : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess=true))
	FDataTableRowHandle ItemID;
	UPROPERTY(ReplicatedUsing= OnRep_Quantity, EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess=true))
	int32 Quantity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess=true))
	float Weight;

public:
	// OnRep
	UFUNCTION(BlueprintCallable)
	void OnRep_Quantity();
	
	UFUNCTION()
	FName GetItemRowName() const { return ItemID.RowName; }
	UFUNCTION()
	int32 GetQuantity() const { return Quantity; }
	UFUNCTION()
	float GetWeight() const { return Weight; }

	UFUNCTION()
	virtual FText LookAt() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractWith(APlayerCharacter* Character);
	virtual void InteractWith_Implementation(APlayerCharacter* Character) override;
	
	
};
