// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnrealCharacter.generated.h"

class UCameraComponent;
class UAimingComponent;

UCLASS()
class UNREALMOTION_API AUnrealCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnrealCharacter();

	// Set Character target Location & Rotation
	void SetCharacterStartLocation(int32 StartLocationID);
	void SetCharacterGameLocation();
	void SetCharacterRotation(FRotator TargetRotation);
	void SetCharacterState(int32 State);

	// For Delayed State Change
	int32 TargetState = 0;

	// Montage
	// UPROPERTY(EditAnywhere, Category= "Montage Reference")
	// TSubclassOf<UAnimMontage> LoadMontage;

	// Accessors
	// FRotator GetCameraRotation();
	// FRotator GetHandOffsetRotation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// References
	class UMainAnimInstance* MainAnimInstance = nullptr;
	UAimingComponent* AimingComponent = nullptr;

	// Character State
	int32 CharacterState = 0;

	// Locations
	FVector StartLocation_0 = FVector(225, 1350, 90.15);
	FVector StartLocation_1 = FVector(225, 1150, 90.15);
	FVector StartLocation_2 = FVector(225, 950, 90.15);
	FVector StartLocation_3 = FVector(225, 750, 90.15);
	FVector GameStartLocation = FVector(851, 1800, 90);

	// Input Functions
	void EscapeMenu();
	void Shoot();
	void Draw();
	void Load();

	// Move Character to Location
	FVector TargetLocation;
	void MoveCharacterTo(FVector SetTargetLocation, float DeltaTime);

	// Character Lerp
	FRotator LookAtRotation;
	void Lerp(float DeltaTime);
	float LITime = 0;
	float LIDuration = 0.5;

	// Camera Clamp
	void CameraRotationClamp();
	FRotator CameraRotation;
	float CameraPitchClamp = 10;
	float CameraYawClamp = 15;

	// Draw
	bool Drawn = false;
	bool Loaded = false;
	void GunLerp(float DeltaTime);

	// Get Hit Location
	FVector HitLocation;
	FVector AimDirection;
	FRotator HandOffsetRotation;
	bool GetHitLocation(FVector& TargetHitLocation);
	//void AimAt(FVector TargetDirection);

};
