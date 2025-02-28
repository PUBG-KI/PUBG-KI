// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Weapon/Weapon_Base.h"
#include "Grenade_Base.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API AGrenade_Base : public AWeapon_Base
{
	GENERATED_BODY()

public:
	AGrenade_Base();

	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* GrenadeMeshComponent;

	UPROPERTY(EditAnywhere)
	USphereComponent* GrenadeSphereCollision;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							 const FHitResult& SweepResult);
	
};
