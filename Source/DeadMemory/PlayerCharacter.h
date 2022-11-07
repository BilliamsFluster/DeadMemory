// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class DEADMEMORY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	APlayerCharacter();
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true));
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true));
	float BaseLookUpAtRate;

protected:
	
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void TurnAtRate(float AxisValue);
	void LookUpAtRate(float AxisValue);

	void AddControllerYawInput(float Value);
	void AddControllerPitchInput(float Value);
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Controller");
	class ACharacterController* CharacterController;
	
public:	
	
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
