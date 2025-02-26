// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor//SupplyDrop/SupplyDrop.h"
#include "Components/BoxComponent.h"
#include "PUBG/Public/GameplayActor/ItemSpawn/ItemSpawnerComponent.h"
#include "Item/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

//#include "Landscape.h"

// Sets default values
ASupplyDrop::ASupplyDrop()
{
	PrimaryActorTick.bCanEverTick = false;

	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropMesh"));
	RootComponent = DropMesh;
	
	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxComponent"));
	CollisionBoxComponent->SetIsReplicated(true);
	CollisionBoxComponent->SetupAttachment(DropMesh);

		
	InteractionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetIsReplicated(true);
	InteractionComponent->SetupAttachment(DropMesh);

	FVector NewBoxCollisionComponentBoxExtent(90.0f, 90.0f, 90.0f);
	CollisionBoxComponent->SetBoxExtent(NewBoxCollisionComponentBoxExtent);

	FVector NewInteractionComponentBoxExtent(50.0f, 50.0f, 50.0f);
	InteractionComponent->SetBoxExtent(NewInteractionComponentBoxExtent);

	
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

	//스폰컴포넌트
	ItemSpawnerComponent = CreateDefaultSubobject<UItemSpawnerComponent>(TEXT("ItemSpawnerComponent"));
	
	// Balloon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Balloon"));
	// Balloon->SetupAttachment(DropMesh);
}

// Called when the game starts or when spawned
void ASupplyDrop::BeginPlay()
{
	Super::BeginPlay();
	
	DropMesh->OnComponentHit.AddDynamic(this, &ASupplyDrop::OnHit);

	// 아이템 블루프린트가 설정되지 않았다면 스폰하지 않음
	//if (!ItemClass) return;

	// 스폰 파라미터 설정
	// FActorSpawnParameters SpawnParams;
	// SpawnParams.Owner = this;
	// SpawnParams.Instigator = GetInstigator();

	// // 보급품의 위치를 기준으로 아이템을 약간 위쪽에 스폰
	// FVector SpawnLocation = GetActorLocation();
	// FRotator SpawnRotation = FRotator::ZeroRotator;

	// 아이템 스폰
	//SpawnedItem = GetWorld()->SpawnActor<AItemBase>(ItemClass, SpawnLocation, SpawnRotation);
	
	//if (SpawnedItem)
	{
		// TestItem2를 부착
		//SpawnedItem->AttachToComponent(DropMesh, FAttachmentTransformRules::KeepWorldTransform);
		// if(HasAuthority())
		// {
		// 	ItemSpawnerComponent->SpawnItems(true,this);
		// }
		
		// SpawnedItem->SetRandomProperties(GetRandomItemRowName());
		//
		// UE_LOG(LogTemp, Warning, TEXT("아이템 %s이(가) 보급품에 추가되었습니다!"), *SpawnedItem->GetName());
	}
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

FName ASupplyDrop::GetRandomItemRowName()
{
	TArray<FName> RowNames = ItemDataTable->GetRowNames();
	FName RandomRowName = RowNames[FMath::RandRange(0,RowNames.Num()-1)];

	//UE_LOG(LogTemp, Warning, TEXT("RandomRowName: %s") , *RandomRowName.ToString());
	
	return RandomRowName;
}

void ASupplyDrop::SpawnSupplyDropItem()
{
	ItemSpawnerComponent->SpawnItems(true,this);
}

FText ASupplyDrop::LookAt()
{
	UE_LOG(LogTemp, Warning, TEXT("ASupplyDropLookAt"));
	return FText::FromString(TEXT("보급품 열기"));
}



