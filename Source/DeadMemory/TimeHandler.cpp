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
#include "Components/PostProcessComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Curves/CurveLinearColor.h"



// Sets default values
ATimeHandler::ATimeHandler()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Time = 0;
	EDayNightCycle = EDayNightCycle::DNC_DayTime;
	DayCount = 0;

	ActorOffset = CreateDefaultSubobject<USceneComponent>(TEXT("ActorLocation"));
	ActorOffset->SetupAttachment(GetRootComponent());
	


}

// Called when the game starts or when spawned
void ATimeHandler::BeginPlay()
{
	Super::BeginPlay();
	TimeOfDay = 0.0f;
	DirectionalLight->SetWorldRotation(FRotator(0, -180, 0));
	GetWorldTimerManager().SetTimer(Clock, this, &ATimeHandler::ClockUpdate, .001, true); //every thousanth of a sec
	SetWeatherCycle(WeatherCycle);

	FOnTimelineFloat UpdateValue;
	FOnTimelineLinearColorStatic ColorUpdateValue;
	UpdateValue.BindUFunction(this, FName("WeatherTimerUpdate")); // binding our function to the update of the timeline

	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindUFunction(this, FName("WeatherTimerFinished")); // binding the finished function to the timeline
	
	/*Add rain curves to timeline*/
	WeatherTimeLine.AddInterpFloat(RainFogCurve, UpdateValue);
	WeatherTimeLine.AddInterpLinearColor(RainFog1Color, ColorUpdateValue);
	WeatherTimeLine.AddInterpLinearColor(RainFog2Color, ColorUpdateValue);
	WeatherTimeLine.AddInterpLinearColor(RainFog3Color, ColorUpdateValue);
	WeatherTimeLine.AddInterpLinearColor(RainFogGlobalColor, ColorUpdateValue);
	
	/*Add snow curves to timeline*/
	WeatherTimeLine.AddInterpFloat(SnowFogCurve, UpdateValue);
	WeatherTimeLine.AddInterpLinearColor(SnowFog1Color, ColorUpdateValue);
	WeatherTimeLine.AddInterpLinearColor(SnowFog2Color, ColorUpdateValue);
	WeatherTimeLine.AddInterpLinearColor(SnowFog3Color, ColorUpdateValue);
	WeatherTimeLine.AddInterpLinearColor(SnowFogGlobalColor, ColorUpdateValue);

	/*Add Normal day conditions curves to timeline*/
	WeatherTimeLine.AddInterpFloat(NormalFogCurve, UpdateValue);
	WeatherTimeLine.AddInterpLinearColor(NormalFog1Color, ColorUpdateValue);
	WeatherTimeLine.AddInterpLinearColor(NormalFog2Color, ColorUpdateValue);
	WeatherTimeLine.AddInterpLinearColor(NormalFog3Color, ColorUpdateValue);
	WeatherTimeLine.AddInterpLinearColor(NormalFogGlobalColor, ColorUpdateValue);

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

		DirectionalLight->SetIntensity(UKismetMathLibrary::FInterpTo(DirectionalLightIntensity, DayLightIntensity, DeltaTime, 4.0f));
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

		DirectionalLight->SetIntensity(UKismetMathLibrary::FInterpTo(DirectionalLightIntensity, NightLightIntensity, DeltaTime, 3.0f));
		SkyLight->GetLightComponent()->SetIntensity(UKismetMathLibrary::FInterpTo(SkyLightIntensity, 0.01, DeltaTime / 4, 5.0f));

		Moon->MoonNightTransition();
	}


}

// Called every frame
void ATimeHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	WeatherTimeLine.TickTimeline(DeltaTime);

}

void ATimeHandler::SetWeatherCycle(EWeatherCycle Cycle)
{
	WeatherCycle = Cycle;
	switch (WeatherCycle)
	{
		case EWeatherCycle::WC_Raining:
		{
			if (RainParticles)
			{
				if (ParticlesComponent)// the particles component manages what spawns, instead of creating 3 we destroy it and reuse it
				{
					ParticlesComponent->DestroyComponent();
				}
				/*Spawn Rain particles at that specific location and scale*/
				ParticlesComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), RainParticles, UKismetMathLibrary::TransformLocation(FTransform(ActorOffset->GetComponentLocation()), WeatherParticleSpawnLocation));
				ParticlesComponent->SetWorldScale3D(RainBoxExtent);
				
				
				
				WeatherTimeLine.Reverse(); // reverses the previous timeline curve for weather
				FTimerHandle WeatherTransition;
				GetWorldTimerManager().SetTimer(WeatherTransition, this, &ATimeHandler::PlayWeatherTimeline, RainDayTransitionTime, false);
				

			}

			

			break;
		}
		case EWeatherCycle::WC_Snowing:
		{
			if (SnowParticles)
			{
				if (ParticlesComponent)// the particles component manages what spawns, instead of creating 3 we destroy it and reuse it
				{
					ParticlesComponent->DestroyComponent();
				}
				/*Spawn Snow particles at that specific location and scale*/
				ParticlesComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SnowParticles, UKismetMathLibrary::TransformLocation(FTransform(ActorOffset->GetComponentLocation()), WeatherParticleSpawnLocation));
				ParticlesComponent->SetWorldScale3D(SnowBoxExtent);
				
				WeatherTimeLine.Reverse();// reverses the previous timeline curve for weather
				FTimerHandle WeatherTransition;
				GetWorldTimerManager().SetTimer(WeatherTransition, this, &ATimeHandler::PlayWeatherTimeline, SnowDayTransitionTime, false);
			}
			

			break;
		}

		case EWeatherCycle::WC_Normal:
		{
			
			if (ParticlesComponent)// the particles component manages what spawns, instead of creating 3 we destroy it and reuse it
			{
				
				ParticlesComponent->DestroyComponent();

			}
			WeatherTimeLine.Reverse();// reverses the previous timeline curve for weather
			FTimerHandle WeatherTransition;
			GetWorldTimerManager().SetTimer(WeatherTransition, this, &ATimeHandler::PlayWeatherTimeline, NormalDayTransitionTime, false); 

			
		}
	}

}

void ATimeHandler::PlayWeatherTimeline()
{
	WeatherTimeLine.PlayFromStart();
}

void ATimeHandler::WeatherTimerUpdate(float Alpha)
{
	if (WeatherParamCollection && RainFogCurve && WeatherCycle == EWeatherCycle::WC_Raining) // set conditions for raining day
	{
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), WeatherParamCollection, FName("FogOpacity"), RainFogCurve->GetFloatValue(Alpha));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), WeatherParamCollection, FName("Fog1Color"), RainFog1Color->GetLinearColorValue(Alpha));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), WeatherParamCollection, FName("Fog2Color"), RainFog2Color->GetLinearColorValue(Alpha));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), WeatherParamCollection, FName("Fog3Color"), RainFog3Color->GetLinearColorValue(Alpha));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), WeatherParamCollection, FName("GlobalFog"), RainFogGlobalColor->GetLinearColorValue(Alpha));
		
	}
	if (WeatherParamCollection && SnowFogCurve && WeatherCycle == EWeatherCycle::WC_Snowing) // set conditions for snowing day
	{
	
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), WeatherParamCollection, FName("FogOpacity"), SnowFogCurve->GetFloatValue(Alpha));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), WeatherParamCollection, FName("Fog1Color"), SnowFog1Color->GetLinearColorValue(Alpha));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), WeatherParamCollection, FName("Fog2Color"), SnowFog2Color->GetLinearColorValue(Alpha));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), WeatherParamCollection, FName("Fog3Color"), SnowFog3Color->GetLinearColorValue(Alpha));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), WeatherParamCollection, FName("GlobalFog"), SnowFogGlobalColor->GetLinearColorValue(Alpha));

	}

	if (WeatherParamCollection && NormalFogCurve && WeatherCycle == EWeatherCycle::WC_Normal) // set conditions for normal day
	{
		
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), WeatherParamCollection, FName("FogOpacity"), NormalFogCurve->GetFloatValue(Alpha));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), WeatherParamCollection, FName("Fog1Color"), NormalFog1Color->GetLinearColorValue(Alpha));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), WeatherParamCollection, FName("Fog2Color"), NormalFog2Color->GetLinearColorValue(Alpha));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), WeatherParamCollection, FName("Fog3Color"), NormalFog3Color->GetLinearColorValue(Alpha));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), WeatherParamCollection, FName("GlobalFog"), NormalFogGlobalColor->GetLinearColorValue(Alpha));

	}

	
	
	
}

void ATimeHandler::WeatherTimerFinished()
{
}
