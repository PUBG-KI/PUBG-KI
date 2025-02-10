

#include "VehicleBase.h"
#include "Components/BoxComponent.h"
#include "Component/PUBGSpringArmComponent.h"
#include "Camera/CameraComponent.h"

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
	CameraBoom->TargetArmLength = 650.0f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 0.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, UPUBGSpringArmComponent::SocketName);

	
	
}

FText AVehicleBase::LookAt()
{
	return FText::FromString("LookAt");
}

void AVehicleBase::InteractWith_Implementation(APlayerCharacter* Character)
{
	
}
