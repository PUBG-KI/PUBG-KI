#include "Vehicle/VehicleBase.h"
#include "Components/BoxComponent.h"
#include "Component/PUBGSpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Component/Input/BaseInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/BaseGameplayTag.h"
#include "DataAsset/Input/DataAsset_InputConfig.h"
#include "ChaosVehicleMovementComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"

void AVehicleBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PlayerController = Cast<APlayerController>(NewController))
	{
		UE_LOG(LogTemp, Warning, TEXT("playercontroller"));
		Client_SetUpLocalPlayerInput(PlayerController);
	}
}

void AVehicleBase::Client_SetUpLocalPlayerInput_Implementation(APlayerController* PlayerController)
{
	if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		UE_LOG(LogTemp, Warning, TEXT("CAST LocalPlayer"));
		UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(Subsystem);
		Subsystem->AddMappingContext(VehicleInputConfigAsset->DefaultMappingContext, 0);
		UE_LOG(LogTemp, Warning, TEXT("ADDMAP"));
	}
}

AVehicleBase::AVehicleBase()
{
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;


	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	InteractionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionComponent"));
	BoxComponent->SetupAttachment(GetMesh(), "RootComponent");
	InteractionComponent->SetupAttachment(GetMesh(), "RootComponent");
	BoxComponent->InitBoxExtent(FVector(40.f));
	InteractionComponent->InitBoxExtent(FVector(20.0f));

	CameraBoom = CreateDefaultSubobject<UPUBGSpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(GetMesh(), "RootComponent");
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, UPUBGSpringArmComponent::SocketName);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(GetMesh(), "RootComponent");
}


void AVehicleBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UBaseInputComponent* BaseInputComponent = CastChecked<UBaseInputComponent>(PlayerInputComponent);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_Throttle,
	                                          ETriggerEvent::Triggered, this, &AVehicleBase::Input_Throttle);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_Throttle,
	                                          ETriggerEvent::Completed, this, &AVehicleBase::Input_Throttle);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_Steering,
	                                          ETriggerEvent::Triggered, this, &AVehicleBase::Input_Steering);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_Steering,
	                                          ETriggerEvent::Completed, this, &AVehicleBase::Input_Steering);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_Break,
	                                          ETriggerEvent::Triggered, this, &AVehicleBase::Input_Break);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_Break,
	                                          ETriggerEvent::Completed, this, &AVehicleBase::Input_Break);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_HandBreak,
	                                          ETriggerEvent::Triggered, this, &AVehicleBase::Input_HandBreak);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_HandBreak,
	                                          ETriggerEvent::Completed, this, &AVehicleBase::Input_HandBreakCompleted);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_VehicleLook,
	                                          ETriggerEvent::Triggered, this, &AVehicleBase::Input_VehicleLook);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_GetOut,
	                                          ETriggerEvent::Started, this, &AVehicleBase::Input_GetOut);
}

void AVehicleBase::Input_Throttle(const FInputActionValue& InputActionValue)
{
	GetVehicleMovement()->SetThrottleInput(InputActionValue.Get<float>());
}

void AVehicleBase::Input_Steering(const FInputActionValue& InputActionValue)
{
	GetVehicleMovement()->SetSteeringInput(InputActionValue.Get<float>());
}

void AVehicleBase::Input_Break(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Brake Inputvalue : %s"), *InputActionValue.ToString());
	GetVehicleMovement()->SetBrakeInput(InputActionValue.Get<float>());
}

void AVehicleBase::Input_HandBreak()
{
	GetVehicleMovement()->SetHandbrakeInput(true);
}

void AVehicleBase::Input_HandBreakCompleted()
{
	GetVehicleMovement()->SetHandbrakeInput(false);
}

void AVehicleBase::Input_VehicleLook(const FInputActionValue& InputActionValue)
{
	AddControllerYawInput(InputActionValue.Get<FVector>().X);
	AddControllerPitchInput(InputActionValue.Get<FVector>().Y);
}

void AVehicleBase::Input_GetOut_Implementation()
{
	if (PlayerCharacter->GetOnTheVehicle() == true)
	{
		PlayerCharacter->SetOnTheVehicle(false);
		APlayerController* PlayerController = Cast<APlayerController>(this->GetController());
		Client_SetUpRemotePlayerInput(PlayerController);
		FRotator VehicleRotator = this->GetCameraBoom()->GetTargetRotation();

		// if (HasAuthority())
		// {
		// 	PlayerController->Possess(PlayerCharacter);
		// 	UE_LOG(LogTemp, Warning, TEXT("Possess1"));
		// }
		Server_Possess_GetOut(PlayerController);
		PlayerController->SetControlRotation(VehicleRotator); // 차량에서 바라보던 카메라 위치로
		//FVector CurrentLocation = GetActorLocation();
		//CurrentLocation.Y -= 150; // Y 값을 -200만큼 감소시킴
		//CurrentLocation.Z += 140;
		// PlayerCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// PlayerCharacter->SetActorLocation(CurrentLocation, true, nullptr, ETeleportType::None); // 액터 로케이션 차옆으로
		// PlayerCharacter->SetActorEnableCollision(true);
		MultiCast_SetActor(PlayerCharacter);
	}
}

bool AVehicleBase::Input_GetOut_Validate()
{
	return true;
}

// void AVehicleBase::Input_GetOut_Implementation()
// {
// 	if (PlayerCharacter->GetOnTheVehicle() == true)
// 	{
// 		PlayerCharacter->SetOnTheVehicle(false);
// 		APlayerController* PlayerController = Cast<APlayerController>(this->GetController());
// 		//PlayerController = Cast<APlayerController>(this->GetController());
// 		//Client_SetUpRemotePlayerInput(PlayerController);
// 		FRotator VehicleRotator = this->GetCameraBoom()->GetTargetRotation();
// 		//PlayerController->UnPossess();
// 		// if (HasAuthority())
// 		// {
// 		// 	PlayerController->Possess(PlayerCharacter);
// 		// 	UE_LOG(LogTemp, Warning, TEXT("Possess1"));
// 		// }
// 		//Server_Input_GetOut(PlayerController);
// 		PlayerController->SetControlRotation(VehicleRotator); // 차량에서 바라보던 카메라 위치로
// 		// FVector CurrentLocation = GetActorLocation();
// 		// CurrentLocation.Y -= 150; // Y 값을 -200만큼 감소시킴
// 		// CurrentLocation.Z += 140;
// 		MultiCast_SetActor(PlayerCharacter);
// 		// PlayerCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
// 		//  PlayerCharacter->SetActorLocation(CurrentLocation, true, nullptr, ETeleportType::None); // 액터 로케이션 차옆으로
// 		//  PlayerCharacter->SetActorEnableCollision(true);
// 	}
// }

// void AVehicleBase::Input_GetOut()
// {
// 	if (PlayerCharacter->GetOnTheVehicle() == true)
// 	{
// 		PlayerCharacter->SetOnTheVehicle(false);
// 		APlayerController* PlayerController = Cast<APlayerController>(this->GetController());
// 		Client_SetUpRemotePlayerInput(PlayerController);
// 		FRotator VehicleRotator = this->GetCameraBoom()->GetTargetRotation();
//
// 		// if (HasAuthority())
// 		// {
// 		// 	PlayerController->Possess(PlayerCharacter);
// 		// 	UE_LOG(LogTemp, Warning, TEXT("Possess1"));
// 		// }
// 		Server_Possess_GetOut(PlayerController);
// 		PlayerController->SetControlRotation(VehicleRotator); // 차량에서 바라보던 카메라 위치로
// 		//FVector CurrentLocation = GetActorLocation();
// 		//CurrentLocation.Y -= 150; // Y 값을 -200만큼 감소시킴
// 		//CurrentLocation.Z += 140;
// 		// PlayerCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
// 		// PlayerCharacter->SetActorLocation(CurrentLocation, true, nullptr, ETeleportType::None); // 액터 로케이션 차옆으로
// 		// PlayerCharacter->SetActorEnableCollision(true);
// 		MultiCast_SetActor(PlayerCharacter);
// 	}
// }

void AVehicleBase::Server_Possess_GetOut_Implementation(APlayerController* PlayerController)
{
	if (HasAuthority())
	{
		PlayerController->Possess(PlayerCharacter);
		UE_LOG(LogTemp, Warning, TEXT("Possess1"));
	}
}

bool AVehicleBase::Server_Possess_GetOut_Validate(APlayerController* PlayerController)
{
	return true;
}

void AVehicleBase::Client_SetUpRemotePlayerInput_Implementation(APlayerController* PlayerController)
{
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (LocalPlayer)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		check(Subsystem);
		Subsystem->RemoveMappingContext(VehicleInputConfigAsset->DefaultMappingContext);
	}
}

FText AVehicleBase::LookAt()
{
	return FText::FromString("LookAt");
}

void AVehicleBase::InteractWith_Implementation(APlayerCharacter* Character) //이거는 서버에서 실행되는 함수임
{
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	PlayerCharacter = Character;
	
	if (PlayerController)
	{
		Character->SetOnTheVehicle(true); //애님인스턴스로 보내기 위함
		PlayerCharacter->WhenGetOntheVehicleUnequippedWeapon();//
		Character->SetActorEnableCollision(false); // 콜리전을 꺼서 차량안으로 들어갈 수 있도록
		SetCharacterCollision(Character);

		FRotator CharacterRotator = Character->GetCameraBoom()->GetTargetRotation();

		Character->AttachToComponent(ArrowComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		//PlayerController->UnPossess(); // 기존 포세스를 해제
		if (HasAuthority())
		{
			PlayerController->Possess(this);
			UE_LOG(LogTemp, Warning, TEXT("Possess"));
		}
		PlayerController->SetControlRotation(CharacterRotator);
	}
	MultiCast_InteractWith(Character);
}

void AVehicleBase::SetCharacterCollision(APlayerCharacter* Character)
{
	Character->SetActorEnableCollision(false);
}

void AVehicleBase::MultiCast_SetActor_Implementation(APlayerCharacter* Character)
{
	SetActor(Character);
}

void AVehicleBase::SetActor(APlayerCharacter* Character)
{
	Character->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Y -= 150; // Y 값을 -200만큼 감소시킴
	CurrentLocation.Z += 140;
	Character->SetActorLocation(CurrentLocation, true, nullptr, ETeleportType::None); // 액터 로케이션 차옆으로
	Character->SetActorEnableCollision(true);
}

// void AVehicleBase::ServerInteractWith_Implementation(APlayerCharacter* Character)
// {
// 	MultiCast_InteractWith(Character);
// }
//
// bool AVehicleBase::ServerInteractWith_Validate(APlayerCharacter* Character)
// {
// 	return true;
// }


void AVehicleBase::MultiCast_InteractWith_Implementation(APlayerCharacter* Character)
{
	SetCharacterCollision(Character);
}
