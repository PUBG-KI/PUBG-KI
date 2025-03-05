// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Grenade/Grenade_Base.h"

#include "Character/PlayerCharacter.h"
#include "Controller/BasePlayerController.h"

AGrenade_Base::AGrenade_Base()
{
	// 물리 활성화
	//WeaponSkeletalMeshComponent->SetSimulatePhysics(true);

	GrenadeSphereCollision = CreateDefaultSubobject<USphereComponent>("GrenadeSphereCollision");
	GrenadeSphereCollision->SetupAttachment(WeaponSkeletalMeshComponent, FName("CollisionSocket"));
	GrenadeSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GrenadeSphereCollision->SetSphereRadius(260.0f);
}

void AGrenade_Base::BeginPlay()
{
	Super::BeginPlay();

	GrenadeSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AGrenade_Base::OnComponentBeginOverlap);
}

// 활성화
void AGrenade_Base::SetHitCollisionActivate()
{
	GrenadeSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetWorld()->GetTimerManager().SetTimer(CollisionSettingTimerHandle, this, &AGrenade_Base::SetHitCollisionDeActivate, 0.2f, false);
}

// 비활성화
void AGrenade_Base::SetHitCollisionDeActivate()
{
	GrenadeSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->Destroy();
}


// 피격시 
void AGrenade_Base::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Error, TEXT("ssssssssssssssssssssssssssssssss"));
		//UE_LOG(LogTemp, Error, TEXT("%s"), *this->GetOwner()->GetName());
	}
	
}
