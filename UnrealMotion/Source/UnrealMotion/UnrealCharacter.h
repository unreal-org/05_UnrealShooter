// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnrealCharacter.generated.h"

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

	// Character State
	int32 CharacterState = 0;

	// Locations
	FVector StartLocation_0 = FVector(275, 1350, 90.15);
	FVector StartLocation_1 = FVector(275, 1150, 90.15);
	FVector StartLocation_2 = FVector(275, 950, 90.15);
	FVector StartLocation_3 = FVector(275, 750, 90.15);

	FVector GameStartLocation = FVector(851, 1800, 90);
	
	// Move Character to Location
	FVector TargetLocation;
	void MoveCharacterTo(FVector SetTargetLocation);

	// Character Lerp
	FRotator LookAtRotation;
	void Lerp(float DeltaTime);
	float LITime = 0;
	float LIDuration = 0.5;


};
