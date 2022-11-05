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
	class UStaticMeshComponent* Moon; // moon mesh

	UPROPERTY(EditDefaultsOnly, Category = "Moon")
	class UStaticMeshComponent* Glow; // glow mesh

	UPROPERTY(EditDefaultsOnly, Category = "Moon")
	class UDirectionalLightComponent* MoonLight; // light for teh moon

	UPROPERTY(EditAnywhere, Category = "Curves")
	class UCurveFloat* MoonCurve; // curve for moon transparency

	UPROPERTY(EditAnywhere, Category = "Curves")
	class UCurveFloat* GlowCurve; // curve to adjust moon glow

	UPROPERTY(EditAnywhere, Category = "Curves")
	class UCurveFloat* MoonLightCurve; // curve for the light intensity of the moon

	FTimeline MoonTimeLine; // create a timeline that will affect the curves

	UFUNCTION()
	void MoonUpdate(float Alpha); // update function for the Moon timeline
	UFUNCTION()
	void MoonFinished(); // finished function for the moon timeline




};
