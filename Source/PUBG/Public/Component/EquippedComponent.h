// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/Guns/Gun_Base.h"
#include "EquippedComponent.generated.h"


class AWeaponItem;
enum class EItemCategory : uint8;
struct FItemSlotStruct;
class AItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUBG_API UEquippedComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquippedComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UDataTable* ItemDataTable;
	
	// 무기 5칸, 방어구(헬멧, 가방, 조끼, 길리슈트) 4칸, 의상 (머리, 안경, 마스크, 셔츠, 겉옷, 한벌 옷, 바지, 신발) 8칸 총 17칸
	// 01 = 주무기, 2 = 보조무기 , 3 = 근접무기, 4 = 투척무기, 5 = 헬멧, 6 = 가방, 7 = 조끼, 8 = 길리, 9 ~ 16 의상
	TArray<AItemBase*> EquippedItems[16];

	TArray<AItemBase*> EquippedMainWeapon; // 현재 장착된 무기, 생성자에서 크기 지정 (2)
	AItemBase* EquippedSubWeapon; // 보조 무기
	AItemBase* MeleeWeapon; // 근접 무기 

public:

	// 1. F를 누르면 아이템이 들어옴
	// 2. 아이템을 통해 카테고리를 나눔
	EItemCategory GetEquippedItemCategory(AItemBase* Item);
	// 3. 카테고리에 따라 인덱스에 할당
	// 4. 이미 인덱스에 값이 있으면 스왑
	void EquipItem(AItemBase* Item);
	void UnEquipItem(AItemBase* Item);

	void EquipSubWeapon();
	void EquipMeleeWeapon();
	void EquipThrow();

	// 메인 무기 빈 슬롯 찾기
	int32 FindSlotMainWeapon();
	// 메인 무기 장착
	void EquipMainWeapon(int32 InIndex, AWeaponItem* MainWeapon);
	UFUNCTION(Server, Reliable)
	void ServerEquipItem(int32 InIndex, AWeaponItem* MainWeapon);
	// 메인 무기 스왑

	
	// 재윤 ======================================
	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* CurrentWeapon;	// 현재 장착 무기

	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* LastCurrentWeapon;// 무기 넣었을 시 넣기 이전 장착했었던 무기

	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* PrimarySlot;		// 첫번쨰 무기 슬롯
	
	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* SecondarySlot;	// 두번째 무기 슬롯

	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* SideArmSlot;		// 권총 슬롯

	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* MeleeSlot;		// 근점 무기 슬롯

	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* ThrowableSlot;	// 투척물 슬롯


	// Getter
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

	// setter
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



