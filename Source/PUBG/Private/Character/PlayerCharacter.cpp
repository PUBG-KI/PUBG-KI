// Fill out your copyright notice in the Description page of Project Settings.

// 캐릭터
#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ActorComponent.h"

// 카메라
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// 입력값 
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/Input/BaseInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAsset/Input/DataAsset_InputConfig.h"

// 어빌리티
#include "AbilitySystem//BaseGameplayTag.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "DataAsset/Startup/DataAsset_StartupBase.h"

APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	
	// 메쉬 부착
	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlayerMeshType"), true);
	if (EnumPtr)
	{
		for (int32 i = 0; i < EnumPtr->GetMaxEnumValue(); ++i)
		{
			FString EnumName = EnumPtr->GetNameStringByIndex(i);
			FName SubobjectName(*EnumName);

			// Skeletal Mesh Component 생성
			USkeletalMeshComponent* NewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(SubobjectName);
			NewMesh->SetupAttachment(GetMesh());
			GetMesh()->SetLeaderPoseComponent(NewMesh);

			// Enum 값으로 맵핑
			EPlayerMeshType EnumValue = static_cast<EPlayerMeshType>(i);
			CharacterEquipmentMap.Add(EnumValue, NewMesh);
		}
	}
	
	// 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->JumpZVelocity = 500.0f; // 
	
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

USkeletalMeshComponent* APlayerCharacter::FindMeshComponent(EPlayerMeshType PlayerMeshType)
{	
	return *CharacterEquipmentMap.Find(PlayerMeshType);
}

void APlayerCharacter::SetMeshComponent(EPlayerMeshType PlayerMeshType, USkeletalMesh* SkeletalMesh)
{	
	if(USkeletalMeshComponent* SkeletalMeshComponent = FindMeshComponent(PlayerMeshType))
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UBaseInputComponent* BaseInputComponent = CastChecked<UBaseInputComponent>(PlayerInputComponent);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Move);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Look);
	
	BaseInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &APlayerCharacter::Input_AbilityInputPressed, &APlayerCharacter::Input_AbilityInputReleased);
}

void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}
	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
    const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
    
    if (LookAxisVector.X != 0.f)
    {
    	AddControllerYawInput(LookAxisVector.X);
    }
    if (LookAxisVector.Y != 0.f)
    {
    	AddControllerPitchInput(LookAxisVector.Y);
    }

}

void APlayerCharacter::Input_AbilityInputPressed(FGameplayTag InputTag) const
{
	BaseAbilitySystemComponent->OnAbilityInputPressed(InputTag);
}

void APlayerCharacter::Input_AbilityInputReleased(FGameplayTag InputTag) const
{
	BaseAbilitySystemComponent->OnAbilityInputReleased(InputTag);
}

void APlayerCharacter::Input_AbilityCTRLPressed(FGameplayTag InputTag) const
{
	//CharacterMovement->Set
}

void APlayerCharacter::Input_AbilityCTRLReleased(FGameplayTag InputTag) const
{
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!StartupData.IsNull())
	{
		if (UDataAsset_StartupBase* LoadedData = StartupData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(GetBaseAbilitySystemComponent());
		}
	}	
}
