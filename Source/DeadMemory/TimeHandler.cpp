// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeHandler.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkyLightComponent.h"
#include "Engine/SkyLight.h"
#include "Moon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


// Sets default values
ATimeHandler::ATimeHandler()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Time = 0;
	EDayNightCycle = EDayNightCycle::DNC_DayTime;
	DayCount = 0;

}

// Called when the game starts or when spawned
void ATimeHandler::BeginPlay()
{
	Super::BeginPlay();
	TimeOfDay = 0.0f;
	DirectionalLight->SetWorldRotation(FRotator(0, -180, 0));
	GetWorldTimerManager().SetTimer(Clock, this, &ATimeHandler::ClockUpdate, .001, true); //every thousanth of a sec
	SetWeatherCycle(WeatherCycle);

}
void ATimeHandler::ClockUpdate()
{



	if (DirectionalLight != nullptr) // Is directional light valid
	{
		if (EDayNightCycle == EDayNightCycle::DNC_NightTime) // if night time then adjust properties for night time
		{



			Time += 1;
			TimeOfDay = UKismetMathLibrary::MapRangeClamped(Time, 0, (NightSpeed * 1000), 12, 24); // if reaches the max amount of seconds time of day will be 1am
			SunRotation = UKismetMathLibrary::MapRangeClamped(TimeOfDay, 12, 24, -180, -360); // get value of time of day and if it is 24 then the sun orientation will change to be rotating to 360 deg
			NightLighting(Time); // change properties for night
			if (TimeOfDay >= 24)
			{
				SunRotation = 0;
				Time = 0;
				TimeOfDay = 1;
				EDayNightCycle = EDayNightCycle::DNC_DayTime; // if it is 1 am then it is the beginning of day
				DayCount += 1;
			}


		}
		if (EDayNightCycle == EDayNightCycle::DNC_DayTime) // if day time then adjust properties for night time
		{



			Time += 1;

			TimeOfDay = UKismetMathLibrary::MapRangeClamped(Time, 0, (DaySpeed * 1000), 1, 12); // if reaches the max amount of seconds time of day will be 6pm
			SunRotation = UKismetMathLibrary::MapRangeClamped(TimeOfDay, 1, 12, 0, -180); // sun will set if 6pm
			DayLighting(Time); //change properties for day
			if (TimeOfDay >= 12)
			{

				Time = 0;
				TimeOfDay = 12;
				EDayNightCycle = EDayNightCycle::DNC_NightTime; // if it is 6pm then it is the beg of night 
			}

		}

		DirectionalLight->SetWorldRotation(FRotator(SunRotation, 0, 0));
	}




}



void ATimeHandler::DayLighting(float DeltaTime)
{
	if (SkyLight != nullptr && Moon != nullptr) // if the moon and the sky light are valid
	{ // we want to adjust properties to create a realistic day scene

		float DirectionalLightIntensity = DirectionalLight->Intensity;
		float SkyLightIntensity = SkyLight->GetLightComponent()->Intensity;

		DirectionalLight->SetIntensity(UKismetMathLibrary::FInterpTo(DirectionalLightIntensity, 10.0, DeltaTime, 4.0f));
		SkyLight->GetLightComponent()->SetIntensity(UKismetMathLibrary::FInterpTo(SkyLightIntensity, 1, DeltaTime / 4, 8.0f));

		Moon->MoonDayTransition();
	}


}
void ATimeHandler::NightLighting(float DeltaTime)
{
	if (SkyLight != nullptr && Moon != nullptr)// if the moon and the sky light are valid
	{

		//we want to adjust properties to create a realistic night scene
		float DirectionalLightIntensity = DirectionalLight->Intensity;
		float SkyLightIntensity = SkyLight->GetLightComponent()->Intensity;

		DirectionalLight->SetIntensity(UKismetMathLibrary::FInterpTo(DirectionalLightIntensity, 0.1, DeltaTime, 3.0f));
		SkyLight->GetLightComponent()->SetIntensity(UKismetMathLibrary::FInterpTo(SkyLightIntensity, 0.01, DeltaTime / 4, 5.0f));

		Moon->MoonNightTransition();
	}


}

// Called every frame
void ATimeHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ATimeHandler::SetWeatherCycle(EWeatherCycle Cycle)
{
	switch (Cycle)
	{
	case EWeatherCycle::WC_Raining:
	{
		if (RainParticles)
		{
			if (ParticlesComponent)
			{
				ParticlesComponent->DestroyComponent();
			}

			ParticlesComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), RainParticles, FVector(942, -26, -125));

		}

		break;
	}
	case EWeatherCycle::WC_Snowing:
	{
		if (SnowParticles)
		{
			if (ParticlesComponent)
			{
				ParticlesComponent->DestroyComponent();
			}

			ParticlesComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SnowParticles, FVector(942, -26, -125));

		}

		break;
	}
	}

}
