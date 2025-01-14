// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "GameplayTagContainer.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UDataAsset_InputConfig;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

UENUM(BlueprintType)
enum class EPlayerMeshType : uint8
{
	// 플레이어 뼈대를 가진 파츠들 나열 ex) 머리, 손, 발, 상의, 하의 등등
	// 바디는 기본 mesh로 설정 후 리더포즈컴포넌트로 다른 파츠들의 애니메이션 따라오게 만듬
	EssentialBody UMETA(DisplayName = "EssentialBody"),
	EssentialFace UMETA(DisplayName = "EssentialFace"),
	EssentialHair UMETA(DisplayName = "EssentialHair"),
	Head UMETA(DisplayName = "Head"),
	Top UMETA(DisplayName = "Top"),
	Bottom UMETA(DisplayName = "Bottom"),
};
/**
 * 
 */
UCLASS()
class PUBG_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

private:
	virtual void BeginPlay() override;
	
public:
#pragma region Mesh
	// 캐릭터가 장착할 파츠들을 맵으로 저장, enum값으로 원하는 파츠 불러올 수 있게 설정
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TMap<EPlayerMeshType, USkeletalMeshComponent*> CharacterEquipmentMap;
	
	UFUNCTION(BlueprintCallable, Category = "Mesh")
	USkeletalMeshComponent* FindMeshComponent(EPlayerMeshType PlayerMeshType);	
	UFUNCTION(BlueprintCallable, Category = "Mesh")
	void SetMeshComponent(EPlayerMeshType PlayerMeshType, USkeletalMesh* SkeletalMesh);
#pragma endregion

	
private:
#pragma region Components
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
#pragma endregion
	
#pragma region Inputs
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(FGameplayTag InputTag);
	void Input_AbilityInputReleased(FGameplayTag InputTag);
#pragma endregion
	
	// Client only
	virtual void OnRep_PlayerState() override;
	
public:
	virtual void PossessedBy(AController* NewController) override;	
};
