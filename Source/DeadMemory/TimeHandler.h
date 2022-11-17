// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "TimeHandler.generated.h"

UENUM(BlueprintType)
enum class EDayNightCycle :uint8 {
	DNC_DayTime   UMETA(DisplayName = "DayTime"),
	DNC_NightTime   UMETA(DisplayName = "NightTime"),
};

UENUM(BlueprintType)
enum class EWeatherCycle :uint8 {
	WC_Snowing   UMETA(DisplayName = "Snowing"),
	WC_Raining   UMETA(DisplayName = "Raining"),
	WC_Normal	 UMETA(DisplayName = "NormalDay"),
};



UCLASS()
class DEADMEMORY_API ATimeHandler : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimeHandler();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void ClockUpdate(); // clock for day and night cycle
	void DayLighting(float DeltaTime); // Function that manipulates day time effects
	void NightLighting(float DeltaTime); // Function that manipulates night time effects

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "WeatherCycle")
	void SetWeatherCycle(EWeatherCycle Cycle);

	void PlayWeatherTimeline();
	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Offsets");
	USceneComponent* ActorOffset;

	
	

	/*Day Night cycle properties*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "DayNightCycle");
	EDayNightCycle EDayNightCycle; // enum for day or night
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "DayNightCycle|LightIntensity");
	float DayLightIntensity = 1.0f; // sets light intensity for the Day

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "DayNightCycle|LightIntensity");
	float NightLightIntensity = 0.1f; // sets light intensity for the night


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "DayNightCycle");
	float TimeOfDay; // current time of day 1 = 1am, 6 = 12pm, 18 = 6pm, 24 = 12am

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "DayNightCycle|Day Time in seconds");
	float DaySpeed; // day speed in seconds
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "DayNightCycle|Night Time in seconds");
	float NightSpeed; // night speed in seconds

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true));
	class UDirectionalLightComponent* DirectionalLight; // sun light

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	class USkyAtmosphereComponent* SkyAtmosphere;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	class ASkyLight* SkyLight;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	class AMoon* Moon;

	FTimerHandle Clock;

	float SunRotation; // keeps track of sun's rotation 
	float Time = 0; // value for clock

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "DayCount");
	int DayCount;// keeps track of how many days have past since the start

	/*Weather System Properties*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "WeatherCycle");
	EWeatherCycle WeatherCycle; // handles weather conditions 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "WeatherCycle|Particles");
	class UNiagaraSystem* RainParticles; // particle system for rain

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "WeatherCycle|Particles");
	FVector RainBoxExtent; // sets how large the rain particles should 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "WeatherCycle|Particles");
	UNiagaraSystem* SnowParticles;
	
	class UNiagaraComponent* ParticlesComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "WeatherCycle|Particles");
	FVector SnowBoxExtent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), meta = (MakeEditWidget = true), Category = "WeatherCycle|Particles");
	FVector WeatherParticleSpawnLocation;

	UPROPERTY(EditAnywhere, Category = "WeatherCycle|RainCurves")
	class UCurveFloat* RainFogCurve; // curve for the fog intensity of rain

	UPROPERTY(EditAnywhere, Category = "WeatherCycle|SnowCurves")
	 UCurveFloat* SnowFogCurve; // curve for the Fog intensity of snow

	/*handles transitions for each weather cycle*/
	UPROPERTY(EditAnywhere, Category = "WeatherCycle|NormalDayCurves")
	float NormalDayTransitionTime = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "WeatherCycle|SnowCurves")
	float SnowDayTransitionTime = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "WeatherCycle|RainCurves")
	float RainDayTransitionTime = 0.f;

	UPROPERTY(EditAnywhere, Category = "WeatherCycle|NormalDayCurves")
	UCurveFloat* NormalFogCurve; // curve for the Fog intensity of snow
	
	/*Rain Fog Curves*/
	UPROPERTY(EditAnywhere, Category = "WeatherCycle|RainCurves")
	class UCurveLinearColor* RainFog1Color;

	UPROPERTY(EditAnywhere, Category = "WeatherCycle|RainCurves")
	UCurveLinearColor* RainFog2Color;

	UPROPERTY(EditAnywhere, Category = "WeatherCycle|RainCurves")
	UCurveLinearColor* RainFog3Color;

	UPROPERTY(EditAnywhere, Category = "WeatherCycle|RainCurves")
	UCurveLinearColor* RainFogGlobalColor;

	/*Snow Fog Curves*/
	UPROPERTY(EditAnywhere, Category = "WeatherCycle|SnowCurves")
	UCurveLinearColor* SnowFog1Color;

	UPROPERTY(EditAnywhere, Category = "WeatherCycle|SnowCurves")
	UCurveLinearColor* SnowFog2Color;

	UPROPERTY(EditAnywhere, Category = "WeatherCycle|SnowCurves")
	UCurveLinearColor* SnowFog3Color;

	UPROPERTY(EditAnywhere, Category = "WeatherCycle|SnowCurves")
	UCurveLinearColor* SnowFogGlobalColor;

	/*Normal Fog Curves*/
	UPROPERTY(EditAnywhere, Category = "WeatherCycle|NormalDayCurves")
	UCurveLinearColor* NormalFog1Color;

	UPROPERTY(EditAnywhere, Category = "WeatherCycle|NormalDayCurves")
	UCurveLinearColor* NormalFog2Color;

	UPROPERTY(EditAnywhere, Category = "WeatherCycle|NormalDayCurves")
	UCurveLinearColor* NormalFog3Color;

	UPROPERTY(EditAnywhere, Category = "WeatherCycle|NormalDayCurves")
	UCurveLinearColor* NormalFogGlobalColor;

	FTimeline WeatherTimeLine; // create a timeline that will affect the curves

	UFUNCTION()
	void WeatherTimerUpdate(float Alpha); // update function for the Moon timeline

	UFUNCTION()
	void WeatherTimerFinished(); // finished function for the moon timeline

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "WeatherCycle");
	class UMaterialParameterCollection* WeatherParamCollection; // handles adjustments to the fog material

	
	
	







};
