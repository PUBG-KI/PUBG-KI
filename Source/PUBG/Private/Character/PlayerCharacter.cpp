// Fill out your copyright notice in the Description page of Project Settings.

// 캐릭터
#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerState/BasePlayerState.h"
#include "Controller/BasePlayerController.h"

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
#include "AbilitySystem/BaseAttributeSet.h"
#include "AnimInstance/BaseAnimInstance.h"
#include "DataAsset/Startup/DataAsset_StartupBase.h"

APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

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

			// Enum 값으로 맵핑
			EPlayerMeshType EnumValue = static_cast<EPlayerMeshType>(i);
			CharacterEquipmentMap.Add(EnumValue, NewMesh);
		}
	}

	// 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	//GetCharacterMovement()->MaxWalkSpeedCrouched = 200.0f;
	
	bIsProne = false;
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
	if (USkeletalMeshComponent* SkeletalMeshComponent = FindMeshComponent(PlayerMeshType))
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UBaseInputComponent* BaseInputComponent = CastChecked<UBaseInputComponent>(PlayerInputComponent);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Move,
	                                          ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Move);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Look,
	                                          ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Look);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Jump,
	                                          ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Jump);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Crouch,ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Crouch);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Prone,ETriggerEvent::Started, this, &APlayerCharacter::Input_Prone);
	//BaseInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &APlayerCharacter::Input_AbilityInputPressed,
	                                          // &APlayerCharacter::Input_AbilityInputReleased);
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

void APlayerCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	Jump();
}
void APlayerCharacter::Input_Crouch(const FInputActionValue& InputActionValue)
{
	bIsCrouched = true;
}

void APlayerCharacter::Input_Prone(const FInputActionValue& InputActionValue)
{
	bIsProne = true;
	if (UBaseAnimInstance* PlayerAnimInstance = Cast<UBaseAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		PlayerAnimInstance->bIsProne = true;
		if (PlayerAnimInstance->bIsProne)
			UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::Input_Prone - InputActionValue"));
		
	}
		
	
}

void APlayerCharacter::Input_AbilityInputPressed(FGameplayTag InputTag)
{
	UE_LOG(LogTemp, Warning, TEXT("Ability Input Pressed"));
	BaseAbilitySystemComponent->OnAbilityInputPressed(InputTag);
}

void APlayerCharacter::Input_AbilityInputReleased(FGameplayTag InputTag)
{
	UE_LOG(LogTemp, Warning, TEXT("Ability Input Released"));
	BaseAbilitySystemComponent->OnAbilityInputReleased(InputTag);
}


void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	if (PS)
	{
		// 서버에 ASC를 설정합니다. 클라이언트는 OnRep_PlayerState()에서 이 작업을 수행합니다.
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		// 편의 속성 함수를 위해 AttributeSetBase를 설정합니다.
		BaseAttributeSet = PS->GetAttributeSetBase();

		// AI에는 PlayerController가 없으므로 확인을 위해 여기에서 다시 초기화할 수 있습니다. PlayerController가 있는 영웅을 두 번 초기화해도 아무런 해가 없습니다.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// 스타트업 데이터 할당
		if (!StartupData.IsNull())
		{
			if (UDataAsset_StartupBase* LoadedData = StartupData.LoadSynchronous())
			{
				LoadedData->GiveToAbilitySystemComponent(BaseAbilitySystemComponent.Get());
			}
		}

		// 나중에 플레이어의 연결을 끊었다가 다시 합류하는 것을 처리하는 경우 다시 합류로 인한 소유권이 속성을 재설정하지 않도록 이를 변경해야 합니다.
		// 지금은 소유 = 생성/재생이라고 가정합니다.
		//InitializeAttributes();

		// 리스폰 관련 특정 작업을 종료합니다.
		//AddStartupEffects();

		// 체력/마나/스태미나를 최대치로 설정합니다. 이는 *Respawn*에만 필요합니다.
		//SetHealth(GetMaxHealth());
		//SetStamina(GetMaxStamina());
	}
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	if (PS)
	{
		// 클라이언트에 대한 ASC를 설정합니다. 서버는 PossessedBy에서 이 작업을 수행합니다.
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// 클라이언트에 대한 ASC 행위자 정보를 초기화합니다. 서버는 새 액터를 보유할 때 ASC를 초기화합니다.
		BaseAbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// 편의 속성 함수를 위해 AttributeSetBase를 설정합니다.
		BaseAttributeSet = PS->GetAttributeSetBase();

		// 플레이어 입력을 AbilitySystemComponent에 바인딩합니다. 잠재적인 경쟁 조건으로 인해 SetupPlayerInputComponent에서도 호출됩니다.
		//BindASCInput();


		// 나중에 플레이어 연결 끊김과 재참여를 처리하는 경우 재참여로 인한 소유권이 속성을 재설정하지 않도록 이를 변경해야 합니다.
		// 지금은 소유 = 생성/재생이라고 가정합니다.
		//InitializeAttributes();

		// ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
		// if (PC)
		// {
		// 	PC->CreateHUD();
		// }
		//
		// // Simulated on proxies don't have their PlayerStates yet when BeginPlay is called so we call it again here
		// InitializeFloatingStatusBar();
		//
		//
		// // Respawn specific things that won't affect first possession.
		//
		// // Forcibly set the DeadTag count to 0
		// AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// 체력/마나/스태미나를 최대치로 설정합니다. 이는 *Respawn*에만 필요합니다.
		//SetHealth(GetMaxHealth());
		//SetStamina(GetMaxStamina());
	}
}

// void APlayerCharacter::SetPlayerStance(EPlayerStance CachedStance)
// {
// 	PlayerStance = CachedStance;
//
// 	switch (PlayerStance)
// 	{
// 	case EPlayerStance::Stand:
// 		GetCharacterMovement()->MaxWalkSpeed = 300.f;
// 		GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
// 		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
// 		break;
// 		
// 	case EPlayerStance::Prone:
// 		GetCharacterMovement()->MaxWalkSpeed = 100.f;
// 		GetCapsuleComponent()->SetCapsuleHalfHeight(30.0f);
// 		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));
// 		break;
// 		
// 	case EPlayerStance::Crouch:
// 		GetCharacterMovement()->MaxWalkSpeed = 200.f;
// 		GetCapsuleComponent()->SetCapsuleHalfHeight(60.0f);
// 		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -63.0f));
// 		break;
// 	}
// }
