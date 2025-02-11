// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLibrary/DataEnum/ItemEnum.h"
#include "BaseLibrary/DataStruct/ItemSlotStruct.h"
#include "Components/ActorComponent.h"
#include "Weapon/Weapon_Base.h"
#include "Weapon/Guns/Gun_Base.h"
#include "InventoryComponent.generated.h"


class AItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUBG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// 변수 영역
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Inventory", meta=(AllowPrivateAccess=true))
	float MaxInventoryWeight; // 50 // 총알의 경우 0.5, 0.4임
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Inventory", meta=(AllowPrivateAccess=true))
	float CurrentInventoryWeight;
	
	UPROPERTY(ReplicatedUsing = OnRep_Content, EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TArray<FItemSlotStruct> Content;

	
	UPROPERTY(ReplicatedUsing= OnRep_Item, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess=true))
	AItemBase* Item;
	UPROPERTY(Replicated, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess=true))
	AItemBase* NearItem;

	UPROPERTY()
	UDataTable* ItemDataTable;

// 함수 영역
public:
	// OnRep
	UFUNCTION()
	void OnRep_Content();
	UFUNCTION()
	void OnRep_Item();
	
	// Setter
	void SetItem(AItemBase* OutItem) { Item = OutItem; }
	UFUNCTION(Server, Reliable)
	void ServerSetItem(AItemBase* OutItem);
	void SetNearItem(AItemBase* OutNearItem) { NearItem = OutNearItem; }
	UFUNCTION(BlueprintCallable)
	void SetContent(TArray<FItemSlotStruct> OutContent) { Content = OutContent; }

	UFUNCTION(Server, Reliable)
	void ServerSetNearItem(AItemBase* OutNearItem);
	UFUNCTION(Server, Reliable)
	void ServerSetContents(const TArray<FItemSlotStruct>& OutContnets);
	
	// Getter
	AItemBase* GetItem() const { return Item; }
	AItemBase* GetNearItem() const { return NearItem; }
	TArray<FItemSlotStruct> GetContent() { return Content; }
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable) 
	void Server_Interact();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable) 
	void Server_InteractItem(AItemBase* OutItemBase); // 바닥에 있는 아이템
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerGetItem();

	UFUNCTION(BlueprintCallable)
	int32 AddToInventory(FName ItemID, int32 Quantity, int32 Weight);
	UFUNCTION(BlueprintCallable)
	int32 FindItemSlot(FName ItemID);
	UFUNCTION(BlueprintCallable)
	void AddToStack(int32 Index, int32 Quantity, int32 Weight);
	UFUNCTION(BlueprintCallable)
	void CreateNewStack(FName ItemID, int32 Quantity, int32 Weight, int32 Index);
	UFUNCTION(BlueprintCallable)
	void AddToLastIndexNewStack(FName ItemID, int32 Quantity, int32 Weight);
	UFUNCTION(BlueprintCallable)
	int32 GetMaxStackSize(FName ItemID);
	// 아이템 카테고리 분류하고 함수 실행
	EItemCategory GetEquippedItemCategory(AItemBase* InItem);
	void InteractionsByCategory(AItemBase* InItem);

	// 아이템 먹었을 때 정렬되는 거 
	UFUNCTION(BlueprintCallable) 
	void UpdateInventory();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerUpdateInventory();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ReplicateContent();

	
	UFUNCTION(BlueprintCallable)
	void PrintContents();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerPrintContents();

	UFUNCTION(BlueprintCallable)
	void TransferSlots();


	
	// 재윤, 디버그용 CurrentWeapon변수
	UPROPERTY(visibleAnywhere, Replicated, Category="Inventory")
	AWeapon_Base* CurrentWeapon;

	UPROPERTY(visibleAnywhere, Replicated, Category="Inventory")
	AWeapon_Base* LastCurrentWeapon;

	UPROPERTY(visibleAnywhere, Replicated, Category="Inventory")
	AWeapon_Base* PrimarySlot;
	
	UPROPERTY(visibleAnywhere, Replicated, Category="Inventory")
	AWeapon_Base* SecondarySlot;

	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* SideArmSlot;		// 권총 슬롯

	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* MeleeSlot;		// 근점 무기 슬롯

	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* ThrowableSlot;	// 투척물 슬롯

	

	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetCurrentWeapon() const {return CurrentWeapon; }
	
	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetLastCurrentWeapon() const { return LastCurrentWeapon; }

	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetPrimarySlotWeapon() const { return PrimarySlot; }

	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetSecondarySlot() const { return SecondarySlot; }

	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetSideArmSlot() const { return SideArmSlot; }

	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetMeleeSlot() const { return MeleeSlot; }

	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetThrowableSlot() const { return ThrowableSlot; }

	//-
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentWeapon(AWeapon_Base* _CurrentWeapon);
	
	UFUNCTION(BlueprintCallable)
	void SetLastCurrentWeapon(AWeapon_Base* _LastCurrentWeapon);

	UFUNCTION(BlueprintCallable)
	void SetPrimarySlotWeapon(AWeapon_Base* _PrimarySlot);
	
	UFUNCTION(BlueprintCallable)
	void SetSecondarySlotWeapon(AWeapon_Base* _Secondary);

	UFUNCTION(BlueprintCallable)
	void SetSideArmSlotWeapon(AWeapon_Base* _SideArm);

	UFUNCTION(BlueprintCallable)
	void SetMeleeSlotWeapon(AWeapon_Base* _Melee);

	UFUNCTION(BlueprintCallable)
	void SetThrowableSlotWeapon(AWeapon_Base* _Throwable);

	

	// ------------- gun base casting 반환
	UFUNCTION(BlueprintCallable)
	AGun_Base* GetCurrentWeapon_GunBase() const { return Cast<AGun_Base>(CurrentWeapon);}

	UFUNCTION(BlueprintCallable)
	AGun_Base* GetLastCurrentWeapon_GunBase() const { return Cast<AGun_Base>(LastCurrentWeapon);}
	
	UFUNCTION(BlueprintCallable)
	AGun_Base* GetPrimary_GunBase() const { return Cast<AGun_Base>(PrimarySlot);}
	
	UFUNCTION(BlueprintCallable)
	AGun_Base* GetSecondary_GunBase() const { return Cast<AGun_Base>(SecondarySlot);}


	
};






