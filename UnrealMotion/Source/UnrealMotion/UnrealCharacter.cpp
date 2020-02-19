// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealCharacter.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainAnimInstance.h"
#include "MainPlayerController.h"
#include "Camera/CameraComponent.h"

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
	CameraComponent = FindComponentByClass<UCameraComponent>();
	
}

// Called every frame
void AUnrealCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CharacterState)
	{
		case 0: // Idle
			break;
		case 1: // Walking
			MoveCharacterTo(TargetLocation, DeltaTime);
			break;
		case 2: // Player Control
			if (CameraComponent) { CameraRotationClamp(); }
			break;
	}

	
}

// Called to bind functionality to input
void AUnrealCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Directional Input
	// PlayerInputComponent->BindAxis("HorizontalAxis", this, &AUnrealCharacter::LookRight);
	// PlayerInputComponent->BindAxis("VerticalAxis", this, &AUnrealCharacter::LookUp);

	// Face Buttons
	PlayerInputComponent->BindAction("EscapeMenu", IE_Pressed, this, &AUnrealCharacter::EscapeMenu);
	// PlayerInputComponent->BindAction("Jump", IE_Released, this, &AHoopzCharacter::JumpReleased);
	// PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &AHoopzCharacter::TurnLeft);
	// PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &AHoopzCharacter::TurnRight);
	// PlayerInputComponent->BindAction("DashOrShot", IE_Pressed, this, &AHoopzCharacter::DashOrShot);
}

// Input Functions
void AUnrealCharacter::EscapeMenu()
{
	// Get Player Controller & Add EscapeMenu to viewport
	if (GetController()) { Cast<AMainPlayerController>(GetController())->EscapeMenuPressed(); }
}

// Camera Rotation Clamp
void AUnrealCharacter::CameraRotationClamp()
{
	FRotator CameraRotation = CameraComponent->GetRelativeRotation();

	CameraRotation.Pitch = FMath::ClampAngle(CameraRotation.Pitch, -CameraPitchClamp, CameraPitchClamp);
    CameraRotation.Yaw = FMath::ClampAngle(CameraRotation.Yaw, -CameraYawClamp, CameraYawClamp);
    CameraRotation.Roll = FMath:: ClampAngle(CameraRotation.Roll, 0, 0);

	CameraComponent->SetRelativeRotation(CameraRotation, false);
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
			// stop ticking
			break;
		case 1: // Trigger Transition to Walking
			if (MainAnimInstance) {
				MainAnimInstance->Walking = true;
				MainAnimInstance->Ready = false;
			}
			break;
		case 2: // Player Control
			if (MainAnimInstance) { MainAnimInstance->Ready = true; }
			
			break;
	}
}

// Move Character to Location
void AUnrealCharacter::MoveCharacterTo(FVector SetTargetLocation, float DeltaTime)
{
	if (!(GetActorLocation().Equals(SetTargetLocation, 3))) {
		FVector TargetUnitDirection = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), SetTargetLocation);
		AddMovementInput(TargetUnitDirection, 0.3, true);
		return;
	}
	if (!(GetActorRotation().Equals(LookAtRotation, 1))) {
		Lerp(DeltaTime);
		return;
	}
	
	SetCharacterState(TargetState);
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
        FRotator TargetRotation = FMath::Lerp(GetActorRotation(), LookAtRotation, LITime * 7);
		SetActorRotation(TargetRotation, ETeleportType::None);
    }
}

