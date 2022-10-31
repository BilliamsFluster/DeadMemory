// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeHandler.generated.h"

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
	void DayNightCycle(float DeltaTime); // function for basic day and night cycle
	void DayLighting(float DeltaTime); // Function that manipulates day time effects
	void NightLighting(float DeltaTime); // Function that manipulates night time effects

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	float DayTime; // used for time of day
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	float NightTime; // used for time of night


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	class UDirectionalLightComponent* DirectionalLight; 
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	class USkyAtmosphereComponent* SkyAtmosphere;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	class ASkyLight* SkyLight;
	
	bool bIsNightTime; // checks if it is night time
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	class AMoon* Moon;

	

};
