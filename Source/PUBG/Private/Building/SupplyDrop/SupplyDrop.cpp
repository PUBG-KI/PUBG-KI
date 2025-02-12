// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/SupplyDrop/SupplyDrop.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

//#include "Landscape.h"

// Sets default values
ASupplyDrop::ASupplyDrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Smoke = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Smoke"));
	// //RootComponent = Smoke;
	// Smoke->SetupAttachment(RootComponent);
	
	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropMesh"));
	DropMesh->SetupAttachment(RootComponent);
	
	//DropMesh->SetNotifyRigidBodyCollision(true);
	
	Balloon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Balloon"));
	Balloon->SetupAttachment(DropMesh);

	
	//DropMesh->OnComponentHit.AddDynamic(this, &ASupplyDrop::OnHit);

	//충돌
	DropMesh->SetSimulatePhysics(true);
	DropMesh->SetEnableGravity(true);
	
	DropMesh->SetNotifyRigidBodyCollision(true);
	
	DropMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	DropMesh->SetLinearDamping(5.0f);
	DropMesh->SetAngularDamping(1.0f);

	DropMesh->BodyInstance.bLockXTranslation = true;
	DropMesh->BodyInstance.bLockYTranslation = true;
	DropMesh->BodyInstance.bLockZTranslation = false;

	//회전 잠금 (X, Y, Z 회전 고정)
	DropMesh->BodyInstance.bLockXRotation = true;
	DropMesh->BodyInstance.bLockYRotation = true;
	DropMesh->BodyInstance.bLockZRotation = true;
}

// Called when the game starts or when spawned
void ASupplyDrop::BeginPlay()
{
	Super::BeginPlay();
	
	DropMesh->OnComponentHit.AddDynamic(this, &ASupplyDrop::OnHit);	
}

// Called every frame
void ASupplyDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASupplyDrop::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
   FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Error, TEXT("Hit"));
	
	if (OtherActor && OtherActor != this) 
	{
		UE_LOG(LogTemp, Warning, TEXT("SupplyDrop hit something: %s"), *OtherActor->GetName());

		DropMesh->SetSimulatePhysics(false); 
		if (ParticleEffect)
		{
			AttachedEffect = UGameplayStatics::SpawnEmitterAttached(
				ParticleEffect,
				DropMesh,       
				NAME_None,      
				FVector::ZeroVector, 
				FRotator::ZeroRotator, 
				EAttachLocation::KeepRelativeOffset,
				true
				
			);

			
			//ParticleEffect->Set

			//GetWorldTimerManager().SetTimer(StopEffectHandle, this, &ASupplyDrop::StopEffet, 1.0f, false, 1.0f);
		}
	}
}

// void ASupplyDrop::StopEffet()
// {
// 	ParticleEffect->bAutoDeactivate = true;
// }


