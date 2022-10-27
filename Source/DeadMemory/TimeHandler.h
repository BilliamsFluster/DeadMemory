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

	FTimerHandle DayNightCycleHandle;
	
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void DayNightCycle();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	float CycleTime;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	FRotator DirectionalLightRotation;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	class UDirectionalLightComponent* DirectionalLight;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true));
	TSubclassOf<class USkyAtmosphereComponent> SkyAtmosphere;

	

	

};
