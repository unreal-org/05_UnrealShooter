// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealCharacter.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainAnimInstance.h"
#include "MainPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "AimingComponent.h"

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
	AimingComponent = FindComponentByClass<UAimingComponent>();
	
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
			CameraRotationClamp();
			GetHitLocation(HitLocation);  	 // Get HitLocation
			if(AimingComponent) { AimDirection = AimingComponent->ReadyAim(HitLocation); } 	 // Get AimDirection using ReadyAim with HitLocation using AimingComponent
			AimAt(AimDirection);   	// Set HandOffsetRotation using AimAt with AimDirection
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
	PlayerInputComponent->BindAction("Draw", IE_Pressed, this, &AUnrealCharacter::Draw);
}

// Get Hit Location in World
bool AUnrealCharacter::GetHitLocation(FVector& HitLocation)
{
	// Get Viewport Size
	int32 ViewportSizeX, ViewportSizeY;
	Cast<AMainPlayerController>(GetController())->GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(ViewportSizeX * 0.5, ViewportSizeY * 0.45);

	// De-project
	FVector HitWorldLocation;
    FVector HitWorldDirection;
	Cast<AMainPlayerController>(GetController())->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, HitWorldLocation, HitWorldDirection);

	// Get Hit Location
	FHitResult HitResult;
    FVector CameraLocation = FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	FVector CameraLookLocation = CameraLocation + HitWorldDirection * 1250;

	// Line Trace to limit range
	if (GetWorld()->LineTraceSingleByChannel(
        HitResult,
        CameraLocation,
        CameraLookLocation,
        ECollisionChannel::ECC_Visibility))
    {
        HitLocation = HitResult.Location;
        return true;
    }

	HitLocation = FVector(0, 0, 0);
    return false;
}

// Get Hand OffsetRotation
void AUnrealCharacter::AimAt(FVector TargetDirection)
{
	// Get Hand rotation in World rotation
	FRotator WorldHandRotation = GetMesh()->GetSocketRotation(FName(TEXT("hand_r")));

	// Get difference between AimDirection rotation & hand rotation
	FRotator AimAsRotator = TargetDirection.Rotation();
	FRotator Difference = AimAsRotator - WorldHandRotation;

	// Set HandOffsetRotation to be added to hand in MainAnimInstance
	HandOffsetRotation = Difference;
}

// Input Functions
void AUnrealCharacter::EscapeMenu()
{
	// Get Player Controller & Add EscapeMenu to viewport
	if (GetController()) { Cast<AMainPlayerController>(GetController())->EscapeMenuPressed(); }
}

void AUnrealCharacter::Draw()
{
	if (!ensure(MainAnimInstance)) { return; }

	// Change Pose Index & Drawn bool
	if (Drawn == false) {
		Drawn = true;
		MainAnimInstance->ReadyIndex = 1;
	} else {
		Drawn = false;
		MainAnimInstance->ReadyIndex = 0;
	}
}

// Get Camera Rotation
FRotator AUnrealCharacter::GetCameraRotation()
{
	FRotator ReturnRotation = CameraRotation + FRotator(6, 0, 0);
	return ReturnRotation;
}

// Get Hand Offset Rotation
FRotator AUnrealCharacter::GetHandOffsetRotation()
{
	return HandOffsetRotation;
}

// Camera Rotation Clamp
void AUnrealCharacter::CameraRotationClamp()
{
	if (!ensure(FindComponentByClass<UCameraComponent>())) { return; }

	CameraRotation = FindComponentByClass<UCameraComponent>()->GetRelativeRotation();

	CameraRotation.Pitch = FMath::ClampAngle(CameraRotation.Pitch, -CameraPitchClamp, CameraPitchClamp);
    CameraRotation.Yaw = FMath::ClampAngle(CameraRotation.Yaw, -CameraYawClamp, CameraYawClamp);
    CameraRotation.Roll = FMath:: ClampAngle(CameraRotation.Roll, 0, 0);

	FindComponentByClass<UCameraComponent>()->SetRelativeRotation(CameraRotation, false);
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

