// Fill out your copyright notice in the Description page of Project Settings.


#include "Moon.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMoon::AMoon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Moon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Moon"));
	Moon->SetupAttachment(GetRootComponent());

	Glow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Glow"));
	Glow->SetupAttachment(Moon);
	
}

// Called when the game starts or when spawned
void AMoon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

