// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Moon.generated.h"

UCLASS()
class DEADMEMORY_API AMoon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	UFUNCTION(BlueprintCallable)
	void SetMoonRotationAndScale(FVector Origin, float Scale);
public:


	void MoonNightTransition();
	void MoonDayTransition();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Moon")
	class UStaticMeshComponent* Moon;

	UPROPERTY(EditDefaultsOnly, Category = "Moon")
	class UStaticMeshComponent* Glow;

	UPROPERTY(EditDefaultsOnly, Category = "Moon")
	class UDirectionalLightComponent* MoonLight;

	UPROPERTY(EditAnywhere, Category = "Curves")
	class UCurveFloat* MoonCurve;

	UPROPERTY(EditAnywhere, Category = "Curves")
	class UCurveFloat* GlowCurve;

	UPROPERTY(EditAnywhere, Category = "Curves")
	class UCurveFloat* MoonLightCurve;

	FTimeline MoonTimeLine;

	UFUNCTION()
	void MoonUpdate(float Alpha);
	UFUNCTION()
	void MoonFinished();




};
