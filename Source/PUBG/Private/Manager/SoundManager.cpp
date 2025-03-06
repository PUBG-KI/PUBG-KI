// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/SoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


USoundManager::USoundManager()
{	
}

void USoundManager::Server_PlaySound2D_Implementation(USoundBase* Sound, float VolumeMultiplier, float PitchMultiplier,
                                                      float StartTime, USoundConcurrency* ConcurrencySettings, const AActor* OwningActor, bool bIsUISound)
{
	Multicast_PlaySound2D(Sound, VolumeMultiplier, PitchMultiplier, StartTime, ConcurrencySettings, OwningActor, bIsUISound);
}

void USoundManager::Multicast_PlaySound2D_Implementation(USoundBase* Sound, float VolumeMultiplier,
	float PitchMultiplier, float StartTime, USoundConcurrency* ConcurrencySettings, const AActor* OwningActor,
	bool bIsUISound)
{
	if (Sound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), Sound, VolumeMultiplier, PitchMultiplier, StartTime, ConcurrencySettings, OwningActor, bIsUISound);
	}
}

void USoundManager::Server_PlaySoundAtLocation_Implementation(USoundBase* Sound, FVector Location,
	float VolumeMultiplier, float PitchMultiplier, float StartTime, class USoundAttenuation* AttenuationSettings,
	USoundConcurrency* ConcurrencySettings, const UInitialActiveSoundParams* InitialParams)
{
	Multicast_PlaySoundAtLocation(Sound, Location, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings, ConcurrencySettings, InitialParams);
}


void USoundManager::Multicast_PlaySoundAtLocation_Implementation(USoundBase* Sound, FVector Location,
	float VolumeMultiplier, float PitchMultiplier, float StartTime, class USoundAttenuation* AttenuationSettings,
	USoundConcurrency* ConcurrencySettings, const UInitialActiveSoundParams* InitialParams)
{
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings, ConcurrencySettings, InitialParams);
	}
}
