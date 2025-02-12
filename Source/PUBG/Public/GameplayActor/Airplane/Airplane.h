// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseLibrary/BaseStructType.h"
#include "Airplane.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTimelineComponent;
class UCurveFloat;

USTRUCT(BlueprintType)
struct FAirplaneData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	AAirplane* AirplaneInstance = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FPlayerAbilitySet> AirplaneAbilities;
};

UCLASS()
class PUBG_API AAirplane : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAirplane();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnRep_ServerLocation();
	
	// 비행기 이동 시작 함수
	void MovePlane(FVector Start, FVector End);
	
	// 타임라인 함수
	UFUNCTION()
	void UpdatePlaneLocation(float Alpha);
	

	UFUNCTION()
	void OnPlaneMoveFinished();
	
	void UpdateCurrentLocation();
	void UpdateIsVisibiltyAirplane(bool bIsVisibiltyAirplane);
	
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles);
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	FAirplaneData AirplaneData;
	
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;

private:
	// 비행기 메쉬
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* PlaneMesh;

	// 카메라 부착용 스프링 암
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArm;

	// 실제 카메라
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTimelineComponent* TimelineComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	float Duration;
	
	// 시작 및 끝 위치
	FVector StartLocation;
	FVector EndLocation;
	
	UPROPERTY(ReplicatedUsing=OnRep_ServerLocation)
	FVector ServerLocation;
};

