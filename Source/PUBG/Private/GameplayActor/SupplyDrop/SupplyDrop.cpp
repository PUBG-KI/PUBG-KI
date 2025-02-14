// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor//SupplyDrop/SupplyDrop.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

//#include "Landscape.h"

// Sets default values
ASupplyDrop::ASupplyDrop()
{
	PrimaryActorTick.bCanEverTick = false;

	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropMesh"));
	RootComponent = DropMesh;
	
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
	
	Balloon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Balloon"));
	Balloon->SetupAttachment(DropMesh);
}

// Called when the game starts or when spawned
void ASupplyDrop::BeginPlay()
{
	Super::BeginPlay();
	
	DropMesh->OnComponentHit.AddDynamic(this, &ASupplyDrop::OnHit);	
}


void ASupplyDrop::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
   FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this) 
	{
		UE_LOG(LogTemp, Warning, TEXT("SupplyDrop hit something: %s"), *OtherActor->GetName());

		DropMesh->SetSimulatePhysics(false); 
		if (ParticleEffect)
		{
			  UParticleSystemComponent* ParticleSystemComponent= UGameplayStatics::SpawnEmitterAttached(
				ParticleEffect,
				DropMesh,       
				NAME_None,      
				FVector::ZeroVector, 
				FRotator::ZeroRotator, 
				EAttachLocation::KeepRelativeOffset,
				true
				
			);

			if (ParticleSystemComponent)
			{				
				DropMesh->OnComponentHit.RemoveDynamic(this, &ASupplyDrop::OnHit);	
			}
		}
	}
}


