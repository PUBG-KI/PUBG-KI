// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Grenade/Grenade_Base.h"

#include "Controller/BasePlayerController.h"

AGrenade_Base::AGrenade_Base()
{
	GrenadeMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("GrenadeMeshComponent");
	GrenadeMeshComponent->SetupAttachment(RootComponent);

	// 물리 활성화
	GrenadeMeshComponent->SetSimulatePhysics(true);

	//GrenadeSphereCollision->CreateDefaultSubobject<USphereComponent>("GrenadeSphereCollision");
	//GrenadeSphereCollision->SetupAttachment(GrenadeMeshComponent);
	//GrenadeSphereCollision->Deactivate();
	//GrenadeSphereCollision->SetSphereRadius(220.0f);

	
}

void AGrenade_Base::BeginPlay()
{
	Super::BeginPlay();

	GrenadeSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AGrenade_Base::OnComponentBeginOverlap);
}

void AGrenade_Base::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABasePlayerController* PlayerController = Cast<ABasePlayerController>(OtherActor->GetInstigatorController()))
	{
		//PlayerController->HitEventServer()
	}
	
}
