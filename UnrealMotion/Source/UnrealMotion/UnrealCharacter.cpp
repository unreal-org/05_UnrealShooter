// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealCharacter.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainAnimInstance.h"
#include "MainPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "AimingComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values
AUnrealCharacter::AUnrealCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	isAlive = true;
}

// Replicated Properties
void AUnrealCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate
    DOREPLIFETIME(AUnrealCharacter, isAlive);
	DOREPLIFETIME(AUnrealCharacter, CameraRotation);
	DOREPLIFETIME(AUnrealCharacter, GunRotation);
}

// Called when the game starts or when spawned
void AUnrealCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	MainAnimInstance = dynamic_cast<UMainAnimInstance*>(GetMesh()->GetAnimInstance());
	AimingComponent = FindComponentByClass<UAimingComponent>();

	//GetMesh()->OnComponentHit.AddDynamic(this, &AUnrealCharacter::OnMeshHit);
	
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
			if (Drawn == true) {
				if (GetHitLocation(HitLocation)) {  	 // Get HitLocation
					if (AimingComponent) { AimDirection = AimingComponent->ReadyAim(HitLocation); } 	 // Get AimDirection using ReadyAim with HitLocation using AimingComponent
					GunLerp(DeltaTime);
				}
			}
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
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AUnrealCharacter::Shoot);
	PlayerInputComponent->BindAction("Load", IE_Pressed, this, &AUnrealCharacter::Load);
	PlayerInputComponent->BindAction("ResetTargets", IE_Pressed, this, &AUnrealCharacter::ResetTargets);
}

// Network
void  AUnrealCharacter::OnRep_isAlive() { OnisAliveUpdate(); }

void AUnrealCharacter::OnisAliveUpdate()
{
    //Client-specific functionality
    if (IsLocallyControlled())
    {
        if (isAlive == true)  
        {
			FString aliveMessage = FString::Printf(TEXT("You are Alive."));
        	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, aliveMessage);
        } else {
			// Trigger RagdollAlpha here
            FString deathMessage = FString::Printf(TEXT("You are Dead."));
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
    }

    //Server-specific functionality
    if (Role == ROLE_Authority)
    {
		if (isAlive == false) {
			FString conditionMessage = FString::Printf(TEXT("%s is Dead."), *GetFName().ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, conditionMessage);
		}
    }

    //Functions that occur on all machines. 
    /*  
        Any special functionality that should occur as a result of damage or death should be placed here. 
    */
}

void AUnrealCharacter::SetisAlive(bool Alive)
{
    if (Role == ROLE_Authority)
    {
		isAlive = Alive;
        OnisAliveUpdate();  // Necessary because the server will not receive the RepNotify - to ensure parallel calls on server & client
    }
}

float AUnrealCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	SetisAlive(false);
    return 0;
}

void AUnrealCharacter::HandleShoot_Implementation()
{
	if (AimingComponent) { AimingComponent->Fire(); }
	Cast<AMainPlayerController>(GetController())->CameraShake();
	// CameraRotation = FindComponentByClass<UCameraComponent>()->GetRelativeRotation();
	FindComponentByClass<UCameraComponent>()->SetRelativeRotation(CameraRotation + FRotator(5, 0, 0), false);
}

void AUnrealCharacter::HandleLoad_Implementation()
{
	if (MainAnimInstance) { MainAnimInstance->PlayLoadMontage(); }
}

void AUnrealCharacter::HandleDraw_Implementation(bool DrawState)
{
	if (DrawState == true) {
		MainAnimInstance->ReadyIndex = 1;
	} else {
		MainAnimInstance->ReadyIndex = 0;
	}
}

void AUnrealCharacter::OnRep_CameraRotation() { OnCameraRotationUpdate(); }

void AUnrealCharacter::OnCameraRotationUpdate()
{
	// Needed?
}

void AUnrealCharacter::SetCameraRotation(FRotator CurrentRotation)
{
	if (Role == ROLE_Authority) {
        CameraRotation = CurrentRotation + FRotator(6, 0, 0);
        OnCameraRotationUpdate();
    }
}

void AUnrealCharacter::OnRep_GunRotation() { OnGunRotationUpdate(); }

void AUnrealCharacter::OnGunRotationUpdate()
{
	// Needed?
}

void AUnrealCharacter::SetGunRotation(FRotator TargetRotation)
{
	if (Role == ROLE_Authority) {
		GunRotation = TargetRotation;
		if (Drawn == true) { FindComponentByClass<UStaticMeshComponent>()->SetWorldRotation(GunRotation, false); }
		else { FindComponentByClass<UStaticMeshComponent>()->SetRelativeRotation(GunRotation, false); }
        
		OnGunRotationUpdate();
    }
}


// Get Hit Location in World
bool AUnrealCharacter::GetHitLocation(FVector& TargetHitLocation)
{
	if (!ensure(GetController())) { return false; }

	AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(GetController());

	// Get Viewport Size
	int32 ViewportSizeX, ViewportSizeY;
	if (MainPlayerController) { MainPlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY); }
	FVector2D ScreenLocation = FVector2D(ViewportSizeX * 0.5, ViewportSizeY * 0.45);

	// De-project
	FVector HitWorldLocation;
    FVector HitWorldDirection;
	if (MainPlayerController) { MainPlayerController->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, HitWorldLocation, HitWorldDirection); }

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
        TargetHitLocation = HitResult.Location;
        return true;
    }

	TargetHitLocation = FVector(0, 0, 0);
    return false;
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
	if (CharacterState != 2) { return; }

	// Change Pose Index & Drawn bool
	if (Drawn == false) {
		Drawn = true;
		// MainAnimInstance->ReadyIndex = 1;
	} else {
		Drawn = false;
		// MainAnimInstance->ReadyIndex = 0;
	}

	HandleDraw(Drawn);
}

void AUnrealCharacter::Shoot()
{
	// Get Player Controller & Add EscapeMenu to viewport
	// if (AimingComponent) {
		if (Drawn == true && Loaded == true) {
			Loaded = false;
			HandleShoot();
			// AimingComponent->Fire();
			// Cast<AMainPlayerController>(GetController())->CameraShake();
			// CameraRotation = FindComponentByClass<UCameraComponent>()->GetRelativeRotation();
			// FindComponentByClass<UCameraComponent>()->SetRelativeRotation(CameraRotation + FRotator(5, 0, 0), false);
		}
	// }
}

void AUnrealCharacter::Load()
{
	if (Drawn == true && Loaded == false) {
		// Play Load Slot Montage
		//if (MainAnimInstance) { MainAnimInstance->PlayLoadMontage(); }
		HandleLoad();
		Loaded = true;
	}
}

void AUnrealCharacter::ResetTargets()
{
	if (Cast<AMainPlayerController>(GetController())->GetMode() == 1) {
		Cast<AMainPlayerController>(GetController())->ResetTargets();
	}
}

// Camera Rotation Clamp
void AUnrealCharacter::CameraRotationClamp()
{
	if (!ensure(FindComponentByClass<UCameraComponent>())) { return; }

	FRotator CurrentCameraRotation = FindComponentByClass<UCameraComponent>()->GetRelativeRotation();

	CurrentCameraRotation.Pitch = FMath::ClampAngle(CurrentCameraRotation.Pitch, -CameraPitchClamp, CameraPitchClamp);
    CurrentCameraRotation.Yaw = FMath::ClampAngle(CurrentCameraRotation.Yaw, -CameraYawClamp, CameraYawClamp);
    CurrentCameraRotation.Roll = FMath:: ClampAngle(CurrentCameraRotation.Roll, 0, 0);

	FindComponentByClass<UCameraComponent>()->SetRelativeRotation(CurrentCameraRotation, false);

	// Server Call
	SetCameraRotation(CurrentCameraRotation);
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

// Gun Lerp
void AUnrealCharacter::GunLerp(float DeltaTime)
{
	// UStaticMeshComponent* Gun = FindComponentByClass<UStaticMeshComponent>();
	// if (!ensure(Gun)) { return; }

	FRotator TargetGunRotation;
	if (Drawn == true) { TargetGunRotation = AimDirection.Rotation(); }
	else { TargetGunRotation = FRotator(0, 0, 0); }

	LITime = 0;
    if (LITime < LIDuration) {
        LITime += DeltaTime;
        FRotator TargetRotation = FMath::Lerp(FindComponentByClass<UStaticMeshComponent>()->GetComponentRotation(), TargetGunRotation, LITime);
		SetGunRotation(TargetRotation);
		// if (Drawn == true) { Gun->SetWorldRotation(TargetRotation, false); }
		// else { Gun->SetRelativeRotation(TargetRotation, false); }
    }
}

// void AUnrealCharacter::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
// {
// 	if (MainAnimInstance) {
// 		MainAnimInstance->RagdollAlpha = 1;
// 		// get PlayerController to UnPossess() & Possess MenuPawn
// 		// call PlayerController OnHit() & return to Title Screen
// 		// Update LeaderBoards
// 	}
// }

