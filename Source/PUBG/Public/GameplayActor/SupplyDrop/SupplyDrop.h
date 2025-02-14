// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SupplyDrop.generated.h"


UCLASS()
class PUBG_API ASupplyDrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASupplyDrop();

	virtual void BeginPlay() override;	

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			   FVector NormalImpulse, const FHitResult& Hit);
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Smoke;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DropMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Balloon;

	
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystemComponent* ParticleComp;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ParticleEffect;
	
	FTimerHandle StopEffectHandle;
};
