// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/InteractDoor.h"

#include "MaterialHLSLTree.h"
#include "Components/BoxComponent.h"
#include "Math/UnitConversion.h"

// Sets default values
AInteractDoor::AInteractDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;
	DoorCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollisionBox"));
	DoorCollisionBox->SetupAttachment(DoorMesh);
	InteractionTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionTriggerBox"));
	InteractionTriggerBox->SetupAttachment(DoorMesh);
}

void AInteractDoor::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindDynamic(this,&AInteractDoor::OpenDoor);
		TimeLine.AddInterpFloat(CurveFloat,TimeLineProgress);
	}
}

void AInteractDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeLine.TickTimeline(DeltaTime);
}

void AInteractDoor::InteractWith_Implementation(APlayerCharacter* Character)
{
	IInteractInterface::InteractWith_Implementation(Character);

	if (IsOpen == false)
	{
		TimeLine.Play();
		IsOpen = true;
	}
	else
	{
		TimeLine.Reverse();
		IsOpen = false;
	}
}

FText AInteractDoor::LookAt()
{
	return FText::FromString("LookAt");
}

void AInteractDoor::OpenDoor(float Value)
{
	DoorMesh->SetWorldRotation(FRotator(0.0f, DoorRotateAngle * Value, 0.0f));
}

	
	
	



