// Fill out your copyright notice in the Description page of Project Settings.


#include "Moon.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DirectionalLightComponent.h"


// Sets default values
AMoon::AMoon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Moon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Moon"));
	Moon->SetupAttachment(GetRootComponent());

	Glow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Glow"));
	Glow->SetupAttachment(Moon);

	MoonLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("MoonLight"));
	MoonLight->SetupAttachment(Moon);


	
}

// Called when the game starts or when spawned
void AMoon::BeginPlay()
{
	Super::BeginPlay();
	FOnTimelineFloat UpdateValue;
	UpdateValue.BindUFunction(this, FName("MoonUpdate")); // binding our function to the update of the timeline

	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindUFunction(this, FName("MoonFinished")); // binding the finished function to the timeline

	/*Adding the curves to the timeline so it can affect them*/
	MoonTimeLine.AddInterpFloat(MoonCurve, UpdateValue); 
	MoonTimeLine.AddInterpFloat(MoonLightCurve, UpdateValue);
	MoonTimeLine.AddInterpFloat(GlowCurve, UpdateValue);
	MoonTimeLine.SetTimelineFinishedFunc(FinishedEvent);


	
}

void AMoon::MoonNightTransition()
{
	MoonTimeLine.Play();
}

void AMoon::MoonDayTransition()
{
	MoonTimeLine.Reverse();
}

void AMoon::SetMoonRotationAndScale(FVector Origin, float Scale)
{
	
	FVector NormalizedLocation = Origin - GetActorLocation();
	UKismetMathLibrary::Vector_Normalize(NormalizedLocation);
	
	SetActorRotation(NormalizedLocation.Rotation());
	SetActorScale3D({ Scale,Scale,Scale });
	



}

// Called every frame
void AMoon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoonTimeLine.TickTimeline(DeltaTime);

}

void AMoon::MoonUpdate(float Alpha)
{
	/*When the moon updates we want to update our materials to the curve value*/
	Moon->SetScalarParameterValueOnMaterials(FName("Opacity"), MoonCurve->GetFloatValue(Alpha));
	Glow->SetScalarParameterValueOnMaterials(FName("GlowAmount"), GlowCurve->GetFloatValue(Alpha));
	MoonLight->SetIntensity(MoonLightCurve->GetFloatValue(Alpha));
}

void AMoon::MoonFinished()
{
}

