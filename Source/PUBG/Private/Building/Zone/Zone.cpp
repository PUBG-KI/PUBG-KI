// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Zone/Zone.h"

#include "Components/SphereComponent.h"

// Sets default values
AZone::AZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	SphereZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>("Sphere Zone Mesh");
	SphereZoneMesh->SetupAttachment(RootComponent);
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(SphereZoneMesh);

	TargetSize = 1000.0f;
	ShrinkSpeed = 1.0f;

}

void AZone::ShrinkZone()
{
	float TempSize = GetMeshWorldScale() - 250.0f;
	TargetSize = FMath::Clamp(TempSize, 0.0f, 1000.0f);
}

float AZone::GetMeshWorldScale() const
{
	if (SphereZoneMesh)
	{
		return SphereZoneMesh->GetComponentScale().X;
	}

	return 1.0f;
}

void AZone::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("%s has overlapped with the component!"), *OtherActor->GetName());
}

// Called when the game starts or when spawned
void AZone::BeginPlay()
{
	Super::BeginPlay();

	TargetSize = GetMeshWorldScale();

	//UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult &, SweepResult);
	//이벤트 연결
	SphereZoneMesh->OnComponentBeginOverlap.AddDynamic(this,&AZone::OnComponentBeginOverlap);
}

void AZone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float CurrentScale = GetMeshWorldScale();
	
	if(CurrentScale <= 0)
	{
		return;
	}

	float InterpToSize = FMath::FInterpTo(CurrentScale, TargetSize, DeltaSeconds, ShrinkSpeed);

	SphereZoneMesh->SetWorldScale3D(FVector(InterpToSize, InterpToSize, 1000.0f));
}

