// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealCharacter.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainAnimInstance.h"

// Sets default values
AUnrealCharacter::AUnrealCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnrealCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	MainAnimInstance = dynamic_cast<UMainAnimInstance*>(GetMesh()->GetAnimInstance());
	
}

// Called every frame
void AUnrealCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CharacterState)
	{
		case 0: // Idle
			Lerp(DeltaTime);
			break;
		case 1: // Walking
			MoveCharacterTo(TargetLocation);
			break;
		case 2: // Player Control

			break;
	}

	
}

// Called to bind functionality to input
void AUnrealCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Set Character State
void AUnrealCharacter::SetCharacterState(int32 State)
{
	if (!ensure(MainAnimInstance)) { return; }

	CharacterState = State;

	switch (State) 
	{
		case 0: // Trigger Transition to Idle Steady
			if (MainAnimInstance) { MainAnimInstance->Walking = false; }
			break;
		case 1: // Trigger Transition to Walking
			if (MainAnimInstance) { MainAnimInstance->Walking = true; }
			break;
		case 2: // Player Control

			break;
	}
}

// Move Character to Location
void AUnrealCharacter::MoveCharacterTo(FVector SetTargetLocation)
{
	if (!(GetActorLocation().Equals(SetTargetLocation, 2.5))) {
		FVector TargetUnitDirection = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), SetTargetLocation);
		AddMovementInput(TargetUnitDirection, 0.3, true);
		return;
	}
	
	SetCharacterState(0);
}

// Set Character Start Location - Moving Back to Start
void AUnrealCharacter::SetCharacterStartLocation(int32 StartLocationID)
{
	switch (StartLocationID)
	{
		case 0: // StartLocation_0
			TargetLocation = StartLocation_0;
			break;
		case 1: // StartLocation_1
			TargetLocation = StartLocation_1;
			break;
		case 2: // StartLocation_2
			TargetLocation = StartLocation_2;
			break;
		case 3: // StartLocation_3
			TargetLocation = StartLocation_3;
			break;
	}

	SetCharacterState(1);
}

// Set Character Move Location
void AUnrealCharacter::SetCharacterGameLocation()
{
	TargetLocation = GameStartLocation;
	SetCharacterState(1);
}

// Set Character Rotation
void AUnrealCharacter::SetCharacterRotation(FRotator TargetRotation)
{
	LookAtRotation = TargetRotation;
}

// Character Lerp
void AUnrealCharacter::Lerp(float DeltaTime)
{
	LITime = 0;
    if (LITime < LIDuration) {
        LITime += DeltaTime;
        FRotator TargetRotation = FMath::Lerp(GetActorRotation(), LookAtRotation, LITime * 3);
		SetActorRotation(TargetRotation, ETeleportType::None);
    }
}
