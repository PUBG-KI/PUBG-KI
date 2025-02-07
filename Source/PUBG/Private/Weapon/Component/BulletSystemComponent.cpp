// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Component/BulletSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon/Projectile/Projectile_Base.h"
#include "Controller/BasePlayerController.h"

// Sets default values for this component's properties
UBulletSystemComponent::UBulletSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// 기본값 초기화
	Gravity = 9.8;
	BulletVelocity = FVector(0, 0, 0);
	StartLocation = FVector(0, 0, 0);
	EndLocation = FVector(0, 0, 0);
	IsBulletInitialized = false;
	AirDensity = 1.225;
}


// Called when the game starts
void UBulletSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	Projectile = Cast<AProjectile_Base>(GetOwner());
	Spawn_ShootBullet(Projectile->VelocityVector, Projectile->BulletSpeed);
	
}


// Called every frame
void UBulletSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SpawnLineTrace_HitResult(DeltaTime);
}

float UBulletSystemComponent::GetDragForce(float Air_Density, float Bullet_Speed, float Cross_Section,
	float Drag_Coefficiency)
{
	return (((((Air_Density * (Bullet_Speed * Bullet_Speed)) * Cross_Section) * Drag_Coefficiency) / 2.0) * -1.0);
}

FVector UBulletSystemComponent::CalculateGravity(FVector& Velocity, float DeltaSecond, float Air_Density)
{
	float DrageForce = GetDragForce(AirDensity, UKismetMathLibrary::VSize(Velocity) / 100, Projectile->BulletCrossSection, Projectile->DragCoefficiency);

	FVector NormalizedVelocity = Velocity;
	UKismetMathLibrary::Vector_Normalize(NormalizedVelocity, 0.0001f);
	
	FVector SecondVector = (((DrageForce / Projectile->BulletMass) * 100.0f) * NormalizedVelocity) * DeltaSecond;
	FVector ThirdVector = UKismetMathLibrary::MakeVector(0, 0, (Gravity * -100.0f) * DeltaSecond);

	return Velocity + SecondVector + ThirdVector;
}

void UBulletSystemComponent::Spawn_ShootBullet(FVector& Velocity, float Bullet_Speed)
{
	BulletVelocity = Velocity * Bullet_Speed;

	StartLocation = GetOwner()->GetActorLocation();
	IsBulletInitialized = true;
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBulletSystemComponent::DestoryBullet, 10.0f, false);
}

void UBulletSystemComponent::SpawnLineTrace_HitResult(float DeltaSecond)
{
	if (IsBulletInitialized)
	{
		DeltaSeconds = DeltaSecond;
		StartLocation = GetOwner()->GetActorLocation();
		EndLocation = (BulletVelocity * DeltaSeconds) + StartLocation;

		FHitResult HitResult;
	
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(Projectile->PlayerCharacter); // 자기 자신은 제외

		GetWorld()->LineTraceSingleByChannel(HitResult,StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel3, CollisionParams);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), StartLocation, EndLocation, FLinearColor::Green, 1, 1.0f);

		// 충돌 판정
		if (HitResult.bBlockingHit)
		{
			
			// tag 설정
			_EventTag = FGameplayTag::RequestGameplayTag("Player.Event.Action.Hit");
			
			// payload 설정
			FGameplayEventData Payload;
			Payload.Instigator = Projectile->PlayerCharacter;
			Payload.Target = HitResult.GetActor();
			Payload.EventMagnitude = Projectile->BulletDamage;

			// UE_LOG(LogTemp, Warning, TEXT("Hit_Actor : %s"), *HitResult.GetActor()->GetName());

			ABasePlayerController* Playercontroller = Cast<ABasePlayerController>(Projectile->PlayerCharacter->GetController());

			if (Playercontroller)
			{
				Playercontroller->HitEventServer(HitResult.GetActor(), _EventTag, Payload);
			}
			
			GetOwner()->Destroy();
		}
		else
		{
			GetOwner()->SetActorLocation(EndLocation);
			BulletVelocity = CalculateGravity(BulletVelocity, DeltaSeconds, AirDensity);
		}
	}
}

void UBulletSystemComponent::DestoryBullet()
{
	GetOwner()->Destroy();
}

