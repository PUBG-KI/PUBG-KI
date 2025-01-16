// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.generated.h"

class UBaseAbilitySystemComponent;
class UBaseAttributeSet;
class UDataAsset_StartupBase;

UCLASS()
class PUBG_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();


	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual bool IsAlive() const;
	/**
	* Getters for attributes from GDAttributeSetBase
	**/
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxStamina() const;
	
	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMoveSpeed() const;

protected:
	// TWeakObjectPtrs 대신 UPROPERTY() 하드 참조를 갖거나 참조가 전혀 없고 호출만 할 수도 있습니다.
	// GetAbilitySystem()을 실행하고 PlayerState 또는 하위 클래스에서 읽을 수 있는 GetAttributeSetBase()를 만듭니다.
	// 사용하기 전에 포인터가 유효한지 테스트했는지 확인하세요.
	// 여기서는 공유된 하드 참조를 원하지 않았고 가져오는 추가 함수 호출도 원하지 않았기 때문에 TWeakObjectPtrs를 선택했습니다.
	// 이 기본 클래스에서 참조할 때마다 PlayerState 또는 하위 클래스의 ASC/AttributeSet입니다.

	TWeakObjectPtr<class UBaseAbilitySystemComponent> BaseAbilitySystemComponent;
	TWeakObjectPtr<class UBaseAttributeSet> BaseAttributeSet;
	
	//동기식으로 데이터를 불러온다
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "StartupData")
	TSoftObjectPtr<UDataAsset_StartupBase> StartupData;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	
public:
	//UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const { return BaseAbilitySystemComponent; }
	//UBaseAttributeSet* GetBaseAttributeSet() const { return BaseAttributeSet; }

	

	
protected:
	// 스폰/리스폰 시 초기화를 위한 캐릭터의 기본 속성입니다.
	// 이는 생성/재생 시 재설정되는 속성 값을 재정의하는 인스턴트 GE입니다.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;
	
	// 이 효과는 시작 시 한 번만 적용됩니다.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	// 캐릭터의 속성을 초기화합니다. 서버에서 실행해야 하지만 클라이언트에서도 실행합니다.
	// 기다릴 필요가 없도록 합니다. 클라이언트에 대한 서버의 복제는 중요하지 않습니다.
	// 값은 동일해야 합니다.
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();
	
    /**
	* 속성에 대한 설정자. Respawning과 같은 특별한 경우에만 이것을 사용하고, 그렇지 않으면 GE를 사용하여 속성을 변경하십시오.
	* 이는 속성의 기본 값을 변경합니다.
	*/

	virtual void SetHealth(float Health);
	virtual void SetStamina(float Stamina);
};
