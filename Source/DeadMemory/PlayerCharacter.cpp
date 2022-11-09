// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "CharacterController.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseTurnRate = 45.f;
	BaseLookUpAtRate = 45.f;
	

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
void APlayerCharacter::TurnAtRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds()); 
	
}
void APlayerCharacter::LookUpAtRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * BaseLookUpAtRate * GetWorld()->GetDeltaSeconds());
	
}
void APlayerCharacter::AddControllerYawInput(float Value)
{
	if (Value != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		ACharacterController* const PC = CastChecked<ACharacterController>(Controller);
		PC->AddYawInput(Value);
	}
}
void APlayerCharacter::AddControllerPitchInput(float Value)
{
	if (Value != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		ACharacterController* const PC = CastChecked<ACharacterController>(Controller);
		PC->AddPitchInput(Value);
		UKismetSystemLibrary::PrintString(GetWorld(), FString::SanitizeFloat(Value));
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

		PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
		PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
		PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
		PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
	}

}

