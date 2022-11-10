// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Offsets");
	USceneComponent* ActorOffset;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Offsets");
	FPostProcessSettings PostProcess;//used for weather effects
	/*Day Night cycle properties*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "DayNightCycle");
	EDayNightCycle EDayNightCycle; // enum for day or night


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "DayNightCycle");
	float TimeOfDay; // current time of day 1 = 1am, 6 = 12pm, 18 = 6pm, 24 = 12am

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "DayNightCycle|Day Time in seconds");
	float DaySpeed; // day speed in seconds
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "DayNightCycle|Night Time in seconds");
	float NightSpeed; // night speed in seconds

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true));
	class UDirectionalLightComponent* DirectionalLight;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	class USkyAtmosphereComponent* SkyAtmosphere;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	class ASkyLight* SkyLight;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	class AMoon* Moon;

	FTimerHandle Clock;

	float SunRotation;
	float Time = 0; // value for clock

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "DayCount");
	int DayCount;

	/*Weather System Properties*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "WeatherCycle");
	EWeatherCycle WeatherCycle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "WeatherCycle|Particles");
	class UNiagaraSystem* RainParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "WeatherCycle|Particles");
	FVector RainBoxExtent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "WeatherCycle|Particles");
	UNiagaraSystem* SnowParticles;
	
	class UNiagaraComponent* ParticlesComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "WeatherCycle|Particles");
	FVector SnowBoxExtent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), meta = (MakeEditWidget = true), Category = "WeatherCycle|Particles");
	FVector WeatherParticleSpawnLocation;

	







};
