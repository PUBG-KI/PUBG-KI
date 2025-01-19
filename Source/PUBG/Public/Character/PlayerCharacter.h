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
	Head UMETA(DisplayName = "Head"),
	Top UMETA(DisplayName = "Top"),
	Bottom UMETA(DisplayName = "Bottom"),
	Body UMETA(DisplayName = "Body"),
	Face UMETA(DisplayName = "Face"),
	Hair UMETA(DisplayName = "Hair"),
};
/**
 * 
 */
UCLASS()
class PUBG_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter(const class FObjectInitializer& ObjectInitializer);

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
	FTimerHandle CollisionTimerHandle;

	//0 ~ 1초동안 시가값으로 아래 변수 처리
	//90 ~ 30
	//-89 ~ -35
	float TimerTime;

	void UpdateProneCollsionSizeAndCharacterZpos();
	void UpdateCrouchCollsionSizeAndCharacterZpos();
	void UpdateProneToCrouchCollsionSizeAndCharacterZpos();
	void UpdateCrouchToProneCollsionSizeAndCharacterZpos();
	
#pragma region Inputs
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_MoveReleased(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Jump(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_Prone(const FInputActionValue& InputActionValue);
	void Input_AbilityInputPressed(FGameplayTag InputTag);
	void Input_AbilityInputReleased(FGameplayTag InputTag);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Crouch();
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Crouch();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Prone();
	// UFUNCTION(Client, Reliable)
	// void Client_Prone();
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Prone();
	
	
	
#pragma endregion
	    
	// Client only
	virtual void OnRep_PlayerState() override;
	
public:
	virtual void PossessedBy(AController* NewController) override;

public:
	// float StandCapsuleHalfHeight;
	// float CrouchCapsuleHalfHeight;
	// float ProneCapsuleHalfHeight;
	// float MeshRelativeLocationStandZ;
	// float MeshRelativeLocationCrouchZ;
	// float MeshRelativeLocationProneZ;

	bool bIsProne;
	bool bIsCrouch;
	bool bAnimationIsPlaying;
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetbAnimationIsPlaying(bool bNewAnimaitonIsPlaying){bAnimationIsPlaying = bNewAnimaitonIsPlaying;}
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool GetIsProne() const { return bIsProne; }
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool GetIsCrouch() const { return bIsCrouch; }

	bool bIsSprint; //Sprint설정 관련
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool GetIsSprint() const {return bIsSprint; }
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool SetIsSprint(bool NewSprint){return bIsSprint = NewSprint;}

	bool bIsWalk; //Walk설정 관련
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool GetbIsWalk() const {return bIsWalk; }
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool SetbIsWalk(bool NewWalk) {return bIsWalk = NewWalk; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character")
	FVector2D MoveForwardVecter;
};




