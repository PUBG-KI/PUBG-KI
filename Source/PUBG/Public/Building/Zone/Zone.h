// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Zone.generated.h"

class UGameplayEffect;
class APlayerCharacter;

UCLASS()
class PUBG_API AZone : public AActor
{
	GENERATED_BODY()
	
public:

	FTimerHandle NotifyTimerHandle;
	FTimerHandle ShrinkTimerHandle;
	
	// Sets default values for this actor's properties
	AZone();

	UFUNCTION(BlueprintCallable,Category="NotifySize")
	void NotifySize();
	
	UFUNCTION()
	void UpdateShrinkZone(float Value);

	UFUNCTION()
	void TimelineFinishedFunction();

	//다음 랜덤 범위
	UFUNCTION()
	FVector GetRandomPointInCircle(FVector OriginCenter,float NextRadius);

	UFUNCTION()
	float GetCurrentRadius();

	UFUNCTION()
	float GetMeshWorldScale() const;

	UFUNCTION()
	void UpdateZoneDamage(APlayerCharacter* PlayerCharacter);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
	
	FTimeline Timeline;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* TimelineComponent;

	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;


private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SphereZoneMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int Level;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int Timer;

	float CurrentRadius;
	float NextRadius;
	
	float TargetScale;
	float CurrentScale;
	
	FVector CurrentLocation;
	FVector NextLocation;
	
};



