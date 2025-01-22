// Fill out your copyright notice in the Description page of Project Settings.

// 캐릭터
#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerState/BasePlayerState.h"
#include "Controller/BasePlayerController.h"

#include "Component/Movement/PlayerMovementComponent.h"

// 카메라
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// 입력값 
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/Input/BaseInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAsset/Input/DataAsset_InputConfig.h"

// 어빌리티
#include "FrameTypes.h"
#include "AbilitySystem//BaseGameplayTag.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AnimInstance/BaseAnimInstance.h"
#include "DataAsset/Startup/DataAsset_StartupBase.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Component/NearArea/NearComponent.h"
#include "Components/WrapBox.h"
#include "Interface/InteractInterface.h"

//
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Widgets/Inventory/InventoryWidget.h"

APlayerCharacter::APlayerCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
	//FollowCamera->bUsePawnControlRotation = false;

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
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	//BaseAttributeSet->SetMoveSpeed(300.f);
	GetCharacterMovement()->MaxWalkSpeed =300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	//GetCharacterMovement()->MaxWalkSpeedCrouched = 200.0f;

	bIsProne = false;
	bAnimationIsPlaying = false;

	// 이준수 
	static ConstructorHelpers::FClassFinder<UInventoryWidget> InventoryWidgetAsset(TEXT("/Game/Blueprint/Widgets/Inventory/WBP_Inventory.WBP_Inventory_C"));

	if (InventoryWidgetAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Widget Loaded"));
		PlayerInventoryClass = InventoryWidgetAsset.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Widget not found"));
	}

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	NearComponent = CreateDefaultSubobject<UNearComponent>(TEXT("Near"));

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 이준수
	if (IsValid(PlayerInventoryClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Widget Loaded"));
		APlayerController* PlayerController = Cast<APlayerController>(GetController()); 
		
		InventoryWidget = Cast<UInventoryWidget>(CreateWidget(PlayerController, PlayerInventoryClass));
		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport();
			InventoryWidget->SetInventoryComponent(InventoryComponent);
			InventoryWidget->GetWrapBox_Inventory()->ClearChildren();
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Widget not found"));
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnComponentBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnComponentEndOverlap);
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

// void APlayerCharacter::UpdateProneCollsionSizeAndCharacterZpos()
// {
// 	if (TimerTime >= 1)
// 	{
// 		TimerTime = 0;
//
// 		GetWorld()->GetTimerManager().ClearTimer(CollisionTimerHandle);
// 		bAnimationIsPlaying = false;
//
// 		return;
// 	}
//
// 	TimerTime += 0.01f;
//
// 	//TODO:: Calc Zpos And CollsionSize
// 	if (bIsProne)
// 	{
// 		GetCapsuleComponent()->SetCapsuleHalfHeight(
// 			FMath::FInterpTo(StandCapsuleHalfHeight, ProneCapsuleHalfHeight, TimerTime, 1.0f));
// 		GetMesh()->SetRelativeLocation(
// 			FVector(0, 0, FMath::FInterpTo(MeshRelativeLocationStandZ, MeshRelativeLocationProneZ, TimerTime, 1.0f)));
// 	}
// 	else if (!bIsProne)
// 	{
// 		GetCapsuleComponent()->SetCapsuleHalfHeight(
// 			FMath::FInterpTo(ProneCapsuleHalfHeight, StandCapsuleHalfHeight, TimerTime, 1.0f));
// 		GetMesh()->SetRelativeLocation(
// 			FVector(0, 0, FMath::FInterpTo(MeshRelativeLocationProneZ, MeshRelativeLocationStandZ, TimerTime, 1.0f)));
// 	}
// }
//
// void APlayerCharacter::UpdateCrouchCollsionSizeAndCharacterZpos()
// {
// 	if (TimerTime >= 1)
// 	{
// 		TimerTime = 0;
//
// 		GetWorld()->GetTimerManager().ClearTimer(CollisionTimerHandle);
// 		bAnimationIsPlaying = false;
//
// 		return;
// 	}
//
// 	TimerTime += 0.05f;
//
// 	//TODO:: Calc Zpos And CollsionSize
// 	if (bIsCrouch)
// 	{
// 		GetCapsuleComponent()->SetCapsuleHalfHeight(
// 			FMath::FInterpTo(StandCapsuleHalfHeight, CrouchCapsuleHalfHeight, TimerTime, 1.0f));
// 		GetMesh()->SetRelativeLocation(FVector(
// 			0, 0, FMath::FInterpTo(MeshRelativeLocationStandZ, MeshRelativeLocationCrouchZ, TimerTime, 1.0f)));
// 	}
// 	else if (!bIsCrouch)
// 	{
// 		GetCapsuleComponent()->SetCapsuleHalfHeight(
// 			FMath::FInterpTo(CrouchCapsuleHalfHeight, StandCapsuleHalfHeight, TimerTime, 1.0f));
// 		GetMesh()->SetRelativeLocation(FVector(
// 			0, 0, FMath::FInterpTo(MeshRelativeLocationCrouchZ, MeshRelativeLocationStandZ, TimerTime, 1.0f)));
// 	}
// }
//
// void APlayerCharacter::UpdateProneToCrouchCollsionSizeAndCharacterZpos()
// {
// 	if (TimerTime >= 1)
// 	{
// 		TimerTime = 0;
// 		GetWorld()->GetTimerManager().ClearTimer(CollisionTimerHandle);
// 		bAnimationIsPlaying = false;
//
//
// 		return;
// 	}
//
// 	TimerTime += 0.05f;
//
// 	//TODO:: Calc Zpos And CollsionSize
//
// 	GetCapsuleComponent()->SetCapsuleHalfHeight(
// 		FMath::FInterpTo(ProneCapsuleHalfHeight, CrouchCapsuleHalfHeight, TimerTime, 1.0f));
// 	GetMesh()->SetRelativeLocation(
// 		FVector(0, 0, FMath::FInterpTo(MeshRelativeLocationProneZ, MeshRelativeLocationCrouchZ, TimerTime, 1.0f)));
// }
//
// void APlayerCharacter::UpdateCrouchToProneCollsionSizeAndCharacterZpos()
// {
// 	if (TimerTime >= 1)
// 	{
// 		TimerTime = 0;
//
// 		GetWorld()->GetTimerManager().ClearTimer(CollisionTimerHandle);
//
// 		bAnimationIsPlaying = false;
// 		return;
// 	}
//
// 	TimerTime += 0.05f;
//
// 	//TODO:: Calc Zpos And CollsionSize
//
// 	GetCapsuleComponent()->SetCapsuleHalfHeight(
// 		FMath::FInterpTo(CrouchCapsuleHalfHeight, ProneCapsuleHalfHeight, TimerTime, 1.0f));
// 	GetMesh()->SetRelativeLocation(
// 		FVector(0, 0, FMath::FInterpTo(MeshRelativeLocationCrouchZ, MeshRelativeLocationProneZ, TimerTime, 1.0f)));
// }

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
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Move,
	                                          ETriggerEvent::Completed, this, &APlayerCharacter::Input_MoveReleased);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Look,
	                                          ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Look);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Jump,
	                                          ETriggerEvent::Started, this, &APlayerCharacter::Input_Jump);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Crouch,
	                                          ETriggerEvent::Started, this, &APlayerCharacter::Input_Crouch);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Prone,
	                                          ETriggerEvent::Started, this, &APlayerCharacter::Input_Prone);
	BaseInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &APlayerCharacter::Input_AbilityInputPressed,
	                                           &APlayerCharacter::Input_AbilityInputReleased);
	

	
}

void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	bUseControllerRotationYaw = false;  
	UBaseAbilitySystemComponent* AbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent()); // 턴 중 Input_Move 들어오면 캔슬 
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->TryCancelAbilityByTag(BaseGameplayTag::Player_Ability_Turn); 
	}
	 const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	
	const FRotator TargetRotation = FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f); // 목표 회전
	FRotator CurrentRotation = GetActorRotation(); // 현재 회전
	
	// 부드럽게 회전 (보간을 통해)
	FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 10.f); // 10.f는 회전 속도, 더 높은 값일수록 빨리 회전
	MoveForwardVecter.Y = MovementVector.Y;
	// 회전 적용
	Server_SetActorRotation(InterpolatedRotation);
	
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

void APlayerCharacter::Server_SetActorRotation_Implementation(FRotator Rotator)
{
	MultiCast_SetActorRotation(Rotator);
}

bool APlayerCharacter::Server_SetActorRotation_Validate(FRotator Rotator)
{
	return true;
}

void APlayerCharacter::MultiCast_SetActorRotation_Implementation(FRotator Rotator)
{	
	SetActorRotation(Rotator);
}

void APlayerCharacter::Input_MoveReleased(const FInputActionValue& InputActionValue)
{
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
		OnMouseMoved(LookAxisVector); // 마우스 무브를통해 턴 조건확인위한..
	}
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	if (bAnimationIsPlaying)
	{
		return;
	}
	
	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	
	if (GetMovementComponent()->IsCrouching()) //크라우칭 상태면
	{
		UnCrouch();
	}
	else if (MovementComponent->RequestToStartProne) //누워있는 상태면
	{
		MovementComponent->StopProne();
	}
	else
	{
		Jump();
	}	
}

void APlayerCharacter::Input_Crouch(const FInputActionValue& InputActionValue)
{	
	if (GetCharacterMovement()->IsFalling() && bAnimationIsPlaying)
	{
		return;
	}

	UPlayerMovementComponent* MovementComponent =  Cast<UPlayerMovementComponent>(GetMovementComponent());
	
	if (GetCharacterMovement()->IsCrouching()) //크라우칭 상태면
	{
		UnCrouch();
		return;
	}	

	if (MovementComponent->RequestToStartProne)
	{
		MovementComponent->StopProne();
	}
	
	Crouch();
}


void APlayerCharacter::Input_Prone(const FInputActionValue& InputActionValue)
{
	if (GetCharacterMovement()->IsFalling() && bAnimationIsPlaying)
	{
		return;
	}

	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	
	if (MovementComponent->RequestToStartProne) //누워있는 상태면
	{
		
		MovementComponent->StopProne();
		
		return;
	}	

	if (GetMovementComponent()->IsCrouching())
	{
		UnCrouch();            
	}

	MovementComponent->StartProne();
	
	
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
		SetHealth(GetMaxHealth());
		SetStamina(0);
	}
}

void APlayerCharacter::OnMouseMoved(FVector2D MouseMovement)
{
	CheckRotationForTurn();
}

void APlayerCharacter::CheckRotationForTurn()
{
	FRotator CurrentRotation = GetActorRotation();
	FRotator AimRotation = GetBaseAimRotation(); // 카메라나 타겟 방향

	FRotator DeltaRotation = CurrentRotation - AimRotation;
	if (DeltaRotation.Yaw >= 180.0f)
	{
		DeltaRotation.Yaw -= 360.0f;
	}
	else if (DeltaRotation.Yaw <= -180.0f)
	{
		DeltaRotation.Yaw += 360.0f;
	}
	if (DeltaRotation.Yaw >= 90.0f || DeltaRotation.Yaw<=-90.f)
	{
		UBaseAbilitySystemComponent* AbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());

		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->TryActivateAbilityByTagToRandom(BaseGameplayTag::Player_Ability_Turn);
		}
	}
}

void APlayerCharacter::StandToProneCameraTimerSet()
{
	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	if (MovementComponent->RequestToStartProne)
	{
		GetWorld()->GetTimerManager().SetTimer(CameraMoveTimerHandle, this, &APlayerCharacter::StandToProneCameraMovement, GetWorld()->GetDeltaSeconds(), true);
	}
}

void APlayerCharacter::StandToProneCameraMovement()
{
		FVector ProneOffset = FVector(0.f, 55.f, 5.f);  // Prone 상태에서의 카메라 위치
		CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, ProneOffset, GetWorld()->GetDeltaSeconds(), 10.f);

	if (CameraBoom->SocketOffset.Equals(ProneOffset, 0.1f))
	{
		GetWorld()->GetTimerManager().ClearTimer(CameraMoveTimerHandle);
	}
}

void APlayerCharacter::ProneToStandCameraTimerSet()
{
	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	if (!MovementComponent->RequestToStartProne)
	{
		GetWorld()->GetTimerManager().SetTimer(CameraMoveTimerHandle, this, &APlayerCharacter::ProneToStandCameraMovement, GetWorld()->GetDeltaSeconds(), true);
	}
	
}

void APlayerCharacter::ProneToStandCameraMovement()
{
		FVector StandOffset = FVector(0.f, 55.f, 65.f);  // Stand 상태에서의 카메라 위치
		CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, StandOffset, GetWorld()->GetDeltaSeconds(), 10.f);

	if (CameraBoom->SocketOffset.Equals(StandOffset, 0.1f))
	{
		GetWorld()->GetTimerManager().ClearTimer(CameraMoveTimerHandle);
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
		SetHealth(GetMaxHealth());
		SetStamina(0);
	}
}

void APlayerCharacter::InputModeUI()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->SetShowMouseCursor(true);
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, InventoryWidget, EMouseLockMode::DoNotLock, true);
	}	
}

void APlayerCharacter::InputModeGame()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->SetShowMouseCursor(false);
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
	}
}

void APlayerCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		BeginOverlapCount +=1;
		UE_LOG(LogTemp, Warning, TEXT("%d"), BeginOverlapCount);
	}
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, OtherActor]()
	{
		FHitResult Hit;
		FVector Start = GetFollowCamera()->K2_GetComponentLocation();
		FVector End = Start + UKismetMathLibrary::GetForwardVector(FollowCamera->K2_GetComponentRotation()) * 380.0f;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		//ActorsToIgnore.Add(TestCharacter);
		ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);

		if (OtherActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceType, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true, FLinearColor(1, 0, 0, 0), FLinearColor (0, 1, 0, 1));
			if (Hit.GetActor() != nullptr)
			{
				if (LookAtActor != Hit.GetActor())
				{
					LookAtActor = Hit.GetActor();
					IInteractInterface* InteractInterface = Cast<IInteractInterface>(LookAtActor);
			
					if (InteractInterface)
					{
						FText result = InteractInterface->LookAt();
						//InteractInterface->InteractWith();
					}
					else
					{
						LookAtActor = nullptr;
						InventoryComponent->SetItem(nullptr);
					}
				}
				
			}
		}
		
	});

	GetWorldTimerManager().SetTimer(BeginOverlapTimerHandle, TimerDelegate, 0.1f, true);

	
	
}

void APlayerCharacter::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	BeginOverlapCount -=1;
	UE_LOG(LogTemp, Warning, TEXT("%d"), BeginOverlapCount);
	LookAtActor = nullptr;
	InventoryComponent->SetItem(nullptr);
	if (BeginOverlapCount == 0)
	{
		
		GetWorldTimerManager().ClearTimer(BeginOverlapTimerHandle);
	}
}

