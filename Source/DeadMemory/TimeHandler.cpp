// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeHandler.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Components/SkyAtmosphereComponent.h"


// Sets default values
ATimeHandler::ATimeHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimeHandler::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(DayNightCycleHandle, this, &ATimeHandler::DayNightCycle, CycleTime, true);
	
}

void ATimeHandler::DayNightCycle()
{
	AddActorLocalRotation(DirectionalLightRotation);
	if (DirectionalLight != nullptr)
	{
		DirectionalLight->AddLocalRotation(DirectionalLightRotation);
	}
	
}

// Called every frame
void ATimeHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

