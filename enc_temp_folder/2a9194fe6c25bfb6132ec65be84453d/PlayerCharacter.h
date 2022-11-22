// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


USTRUCT(BlueprintType)
struct FCharacterAttributes
{
	GENERATED_BODY()
private:
	UPROPERTY()
	float Health;

	UPROPERTY()
	float MaxHealth;

	UPROPERTY()
	float Level;

public:
	
	FORCEINLINE float GetCharacterHealth() const { return Health; }
	FORCEINLINE void SetCharacterHealth(float InHealth) { Health = InHealth; }

	FORCEINLINE float GetCharacterMaxHealth() const { return MaxHealth; }
	FORCEINLINE void SetCharacterMaxHealth(float InMaxHealth) { MaxHealth = InMaxHealth; }

	FORCEINLINE float GetCharacterLevel() const { return Level; }
	FORCEINLINE void SetCharacterLevel(float InLevel) { Level = InLevel; }


};

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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	class USpringArmComponent* CameraBoom; // cameraboom for ataching camera to player

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	class UCameraComponent* FollowCamera; // camera that follows the player

public:
	FORCEINLINE  USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE  UCameraComponent* GetFollowCamera() const { return FollowCamera; }
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
