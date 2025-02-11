// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class ALandscape;

#include "SupplyDrop.generated.h"


UCLASS()
class PUBG_API ASupplyDrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASupplyDrop();
	
	//랜드스케이프
	// UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Landscape")
	// ALandscape* TargetLandscape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<ALandscape> TargetLandscape;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DropMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Balloon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};


