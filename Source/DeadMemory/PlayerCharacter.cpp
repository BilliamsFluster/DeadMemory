// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "CharacterController.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	if ((Controller != nullptr) && AxisValue != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator{ 0, Rotation.Yaw, 0 };
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
	
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	if ((Controller != nullptr) && AxisValue != 0.0f)
	{

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator{ 0, Rotation.Yaw, 0 };

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, AxisValue);
	}
}



// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
		PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

		PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	}

}

