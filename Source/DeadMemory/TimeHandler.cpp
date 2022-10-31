// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeHandler.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkyLightComponent.h"
#include "Engine/SkyLight.h"
#include "Moon.h"


// Sets default values
ATimeHandler::ATimeHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsNightTime = false;

}

// Called when the game starts or when spawned
void ATimeHandler::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void ATimeHandler::DayNightCycle(float DeltaTime)
{
	
	if (DirectionalLight != nullptr)
	{
		
		DirectionalLight->GetComponentRotation().Pitch < 0 ? bIsNightTime = false : bIsNightTime = true;
		if (bIsNightTime)
		{
			NightLighting(NightTime * DeltaTime);
			FRotator LightRotation = { 0, (NightTime * DeltaTime), 0 };
			DirectionalLight->AddLocalRotation(LightRotation);
		}
		else if (!bIsNightTime)
		{
			DayLighting(DayTime * DeltaTime);
			FRotator LightRotation = { 0, (DayTime * DeltaTime), 0 };
			DirectionalLight->AddLocalRotation(LightRotation);
		}
		
	}
	
	
}

void ATimeHandler::DayLighting(float DeltaTime)
{
	if (SkyLight == nullptr ) return;
	float DirectionalLightIntensity = DirectionalLight->Intensity;
	float SkyLightIntensity = SkyLight->GetLightComponent()->Intensity;

	DirectionalLight->SetIntensity(UKismetMathLibrary::FInterpTo(DirectionalLightIntensity, 10.0, DeltaTime, 4.0f));
	SkyLight->GetLightComponent()->SetIntensity(UKismetMathLibrary::FInterpTo(SkyLightIntensity, 1, DeltaTime / 4, 8.0f));
	if(Moon == nullptr) return
	Moon->MoonDayTransition();

}
void ATimeHandler::NightLighting(float DeltaTime)
{
	if (SkyLight == nullptr) return;
	float DirectionalLightIntensity = DirectionalLight->Intensity;
	float SkyLightIntensity = SkyLight->GetLightComponent()->Intensity;

	DirectionalLight->SetIntensity(UKismetMathLibrary::FInterpTo(DirectionalLightIntensity,0.1,DeltaTime,3.0f));
	SkyLight->GetLightComponent()->SetIntensity(UKismetMathLibrary::FInterpTo(SkyLightIntensity, 0.01, DeltaTime / 4, 5.0f));
	if (Moon == nullptr) return
	Moon->MoonNightTransition();
	
}

// Called every frame
void ATimeHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DayNightCycle(DeltaTime);

}

