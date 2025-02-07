// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Zone/Zone.h"
#include "Character/PlayerCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "BaseLibrary/BaseDebugHelper.h"
#include "Chaos/ChaosPerfTest.h"
#include "Components/PostProcessComponent.h"
#include "Components/SphereComponent.h"
#include "BaseLibrary/BaseFunctionLibrary.h"

// Sets default values
AZone::AZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	SphereZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>("Sphere Zone Mesh");
	SphereZoneMesh->SetupAttachment(RootComponent);
	
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->SetupAttachment(SphereZoneMesh);

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	
}

void AZone::NotifySize()
{
	// 사이즈 알리기
	UE_LOG(LogTemp, Warning, TEXT("Notify!"));

	//현재 원의 반지름
	CurrentRadius = GetCurrentRadius();	
	//현재 원의 중심
	CurrentLocation = GetActorLocation();

	//다음 원의 반지름
	NextRadius = CurrentRadius * 0.5f;
	
	//다음 원의 중심
	NextLocation = GetRandomPointInCircle(CurrentLocation,CurrentRadius - NextRadius);
	
	//현재 원의 스케일
	CurrentScale = GetActorScale().X;
	//다음 원의 스케일
	TargetScale = FMath::Clamp(CurrentScale * 0.5f,0,CurrentScale);

	// UE_LOG(LogTemp, Warning, TEXT("CurrentRadius: %f") , CurrentRadius);
	// UE_LOG(LogTemp, Warning, TEXT("CurrentLocation: %s") , *CurrentLocation.ToString());
	// UE_LOG(LogTemp, Warning, TEXT("NextRadius: %f") , NextRadius);
	// UE_LOG(LogTemp, Warning, TEXT("NextLocation: %s") , *NextLocation.ToString());

	
	if(CurrentScale <= 0)
	{
		return;
	}

	if (CurveFloat)
	{
		//줄어드는 함수 호출
		UE_LOG(LogTemp, Warning, TEXT("ShrinkStart"));
		TimelineComponent->PlayFromStart();
	}

}

float AZone::GetMeshWorldScale() const
{
	if (SphereZoneMesh)
	{
		return GetActorScale().X;
		//SphereZoneMesh->GetStaticMesh()->GetBounds().Exte;
	}

	return 1.0f;
}

void AZone::UpdateZoneDamage(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		UBaseAbilitySystemComponent* ASC = Cast<UBaseAbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());

		FGameplayEffectSpecHandle GameplayEffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level,ASC->MakeEffectContext());
		ASC->ModifyGameplayEffectLevel(GameplayEffectSpecHandle, Level);		
	}
}

void AZone::UpdateShrinkZone(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Update!  CurrentSize: %f") , CurrentSize);
	
	float TempSize = FMath::Lerp(CurrentScale, TargetScale, Value);
	FVector TempLocation = FMath::Lerp(CurrentLocation, NextLocation, Value);

	SetActorScale3D(FVector(TempSize, TempSize ,GetActorScale().Z));
	SetActorLocation(TempLocation);
	
	
	UE_LOG(LogTemp, Warning, TEXT("Update!  TempSize: %f") , TempSize);
	UE_LOG(LogTemp, Warning, TEXT("Update!  TempLocation: %s") , *TempLocation.ToString());

}

void AZone::TimelineFinishedFunction()
{
	//UpdateShrinkZone(1.0f);

	//다음 원의 중심
	NextLocation = FVector::ZeroVector;
		//다음 원의 반지름
	NextRadius = 0.f;
	
	UE_LOG(LogTemp, Warning, TEXT("Finished Event Called."));

	GetWorldTimerManager().SetTimer(NotifyTimerHandle, this, &AZone::NotifySize, 1.0f, false, 1.0f);

	Level++;

	TArray<APlayerCharacter*> PlayerCharacters = UBaseFunctionLibrary::GetAllPlayersInWorld(GetWorld());
	for (APlayerCharacter* PlayerCharacter : PlayerCharacters)
	{
		UpdateZoneDamage(PlayerCharacter);		
	}	
}

FVector AZone::GetRandomPointInCircle(FVector OriginCenter,float RandomRange) //랜덤위치
{
	float DistanceFromCenter = FMath::RandRange(0.0f, RandomRange);

	float Theta = FMath::RandRange(0.0f, 2 * PI);

	float X = OriginCenter.X + DistanceFromCenter * FMath::Cos(Theta);
	float Y = OriginCenter.Y + DistanceFromCenter * FMath::Sin(Theta);

	FVector RandomPoint = FVector(X, Y, OriginCenter.Z);
	
	return RandomPoint;
}

float AZone::GetCurrentRadius()
 {
 	float MeshSizeX = SphereZoneMesh->GetStaticMesh()->GetBounds().BoxExtent.X;
	float Radius = MeshSizeX / 2;
 
 	return Radius;
 }

void AZone::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("%s has overlapped with the component!"), *OtherActor->GetName());

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (PlayerCharacter)
	{
		UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
		
		if (ASC)
		{
			//적용할 대상, 적용할 게임 이펙트, 레벨, 이펙트 컨텍스트 
			ASC->RemoveActiveGameplayEffectBySourceEffect(GameplayEffectClass,nullptr,-1);

			PlayerCharacter->PostProcessComponent->SetVisibility(false);
		}
	}
}

void AZone::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("%s has first Endoverlapped with the component!"), *OtherActor->GetName());
	
	 	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
			if (ASC)
			{
				UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();  
				//적용할 대상, 적용할 게임 이펙트, 레벨, 이펙트 컨텍스트 
				ASC->ApplyGameplayEffectToSelf(GameplayEffect, Level, ASC->MakeEffectContext());

				PlayerCharacter->PostProcessComponent->SetVisibility(true);
			}
		}
}

// Called when the game starts or when spawned
void AZone::BeginPlay()
{
	Super::BeginPlay();

	TargetScale = GetMeshWorldScale();
	
	//이벤트 연결
	SphereZoneMesh->OnComponentBeginOverlap.AddDynamic(this,&AZone::OnComponentBeginOverlap);
	SphereZoneMesh->OnComponentEndOverlap.AddDynamic(this,&AZone::OnComponentEndOverlap);

	GetWorldTimerManager().SetTimer(NotifyTimerHandle, this, &AZone::NotifySize, 1.0f, false, 1.0f);

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this,&AZone::UpdateShrinkZone);
		TimelineComponent->AddInterpFloat(CurveFloat,TimelineProgress);
		
		//타임라인 완료 시
		FOnTimelineEvent TimelineFinishedEvent;
		TimelineFinishedEvent.BindDynamic(this,&AZone::TimelineFinishedFunction);
		//BindDynamic(this,&AZone::TimelineFinishedFunction);
		//BindUFunction(this, FName("TimelineFinishedFunction"));
		TimelineComponent->SetTimelineFinishedFunc(TimelineFinishedEvent);

		//TimelineComponent->SetLooping(true);
		UE_LOG(LogTemp, Warning, TEXT("Binding Findshed!"));
		
	}
}

void AZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TimelineComponent.Tick(DeltaTime);
}

