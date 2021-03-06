// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPawn.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"
#include "UnrealCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/UnrealString.h"
#include "GameFramework/GameState.h"


// Sets default values
AMenuPawn::AMenuPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMenuPawn::BeginPlay()
{
	Super::BeginPlay();

	if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true).Equals(FString("ShowdownMap"), ESearchCase::IgnoreCase))
	{
		Showdown = true;

		// Check GetGameState Player count to see if host or client
		int32 playercount = UGameplayStatics::GetGameState(GetWorld())->PlayerArray.Num();

		if (playercount == 1) {
			// if host
			TargetLocation = FVector(850, 1910, 135);
			LookAtRotation = FRotator(0, -90, 0);

			SetActorRotation(LookAtRotation, ETeleportType::None);
			SetActorLocation(TargetLocation, false);
		} else {
			// if client
			TargetLocation = FVector(850, -160, 135);
			LookAtRotation = FRotator(0, 90, 0);

			SetActorRotation(LookAtRotation, ETeleportType::None);
			SetActorLocation(TargetLocation, false);
		}
	}
	else {
		// Get Spline Component references
		for (TActorIterator<AActor> It(GetWorld()); It; ++It)
		{
			class AActor* Target = *It;
			if (Target && Target->GetName() == FString("MainMenuSpline_BP_2")) {
				MainMenuSpline = Target->FindComponentByClass<USplineComponent>(); 
			}
			if (Target && Target->GetName() == FString("CharacterSelectSpline_BP_2")) {
				CharacterSelectSpline = Target->FindComponentByClass<USplineComponent>();
			}
		}

		// Get & Set Initial values
		if (MainMenuSpline) {
			TargetLocation = MainMenuSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);  // Set MenuPawn Initial TargetLocation for Interp
			SplineLength = MainMenuSpline->GetSplineLength();    // Get SplineLength to Loop MenuPawn TargetLocation
			TargetSpline = MainMenuSpline;     // Set TargetSpline to MainMenuSpline
			SplineDistance = 0;    // Start at Length 0 on Main Menu Spline
		}

		// Set MenuPawn variables
		SetActorLocation(TargetLocation, false);
		CurrentState = 0;
		InterpSpeed = 3;
	}
}

// Called every frame
void AMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Showdown == false) {
		switch (CurrentState)
		{
			case 0: // Main Menu
				SplineDistance = UKismetMathLibrary::GenericPercent_FloatFloat(SplineDistance + DeltaTime * 30, SplineLength);   // Modulo to reset Distance after SplineLength
				if (!ensure(TargetSpline)) { return; }   // Double ensure due to crashing
				if (TargetSpline) { TargetLocation = TargetSpline->GetWorldLocationAtDistanceAlongSpline(SplineDistance); }  // Get Location at SplineDistance
				LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(830, 870, 0));  // Look at Center of arena
				break;
			case 1: // Character Select
				if (!ensure(TargetSpline)) { return; } // Double ensure due to crashing
				if (TargetSpline) { TargetLocation = TargetSpline->GetLocationAtSplinePoint(CharacterID, ESplineCoordinateSpace::World); }  // Set Location at Spline Point
				LookAtRotation = FRotator(0, 180, 0); // Look in direction of character
				break;
			case 2: // Mode Select
				break;
			case 3: // Practice Mode & Showdown
				if (CharacterReference) {
					FVector Target = CharacterReference->GetActorLocation();
					Target.Z += 15;
					LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);  // Look at Center of arena
				}
				break;
		}

		LerpAndInterp(DeltaTime);
	}

	//LerpAndInterp(DeltaTime);
}

// Lerp & Interp
void AMenuPawn::LerpAndInterp(float DeltaTime)
{
	LITime = 0;
    if (LITime < LIDuration)
    {
        LITime += DeltaTime;
        FRotator MenuPawnRotation = FMath::Lerp(GetActorRotation(), LookAtRotation, LITime * 4.5);
		FVector MenuPawnLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, InterpSpeed);

		SetActorRotation(MenuPawnRotation, ETeleportType::None);
		SetActorLocation(MenuPawnLocation, false);
    }

	// SetActorRotation(LookAtRotation, ETeleportType::None);
	// SetActorLocation(TargetLocation, false);
}

//////////////////////////// Accessors & Setters ////////////////////////////////
int32 AMenuPawn::GetCharacterID()
{
	return CharacterID;
}

void AMenuPawn::SetCharacter(AUnrealCharacter* TargetCharacter)
{
	CharacterReference = TargetCharacter;
}

//////////////////////////// UI Functions ////////////////////////////////
void AMenuPawn::OnClickedStart()
{
	TargetSpline = CharacterSelectSpline;   // Redirect TargetSpline pointer
	CurrentState = 1;    // Switch state to main menu
	CharacterID = 0;   // Start at Character 0
	InterpSpeed = 10;   // Menu pawn Location InterpSpeed
}

void AMenuPawn::OnClickedReturnToTitle()
{
	TargetSpline = MainMenuSpline;   // Redirect TargetSpline pointer
	CurrentState = 0;     // Switch state to character select
	CharacterID = 0;   // Start at Character 0
	InterpSpeed = 3;   // Menu pawn Location InterpSpeed
}

void AMenuPawn::OnClickedCharacterNext()
{
	CharacterID = abs(UKismetMathLibrary::GenericPercent_FloatFloat(CharacterID + 1, 4));  // To loop between characters
}

void AMenuPawn::OnClickedCharacterPrev()
{
	if (CharacterID == 0) { CharacterID = 3; }
	else { CharacterID = abs(UKismetMathLibrary::GenericPercent_FloatFloat(CharacterID - 1, 4)); }   // To loop between characters
}

void AMenuPawn::OnClickedCharacterConfirm()
{
	CurrentState = 2;    // Switch state to mode select
	TargetLocation += FVector(-100, 0, 0);    // Zoom in Camera
}

void AMenuPawn::OnClickedModeSelectReturn()
{
	CurrentState = 1;    // Switch state to mode select
}

void AMenuPawn::OnClickedModeSelectPractice()
{
	CurrentState = 3;
	TargetLocation = FVector(850, 1650, 135);
}

void AMenuPawn::OnClickedModeSelectShowdown()
{
	CurrentState = 3;
	TargetLocation = FVector(850, 1900, 135);
}

void AMenuPawn::OnClickedModeReadyStart()
{
	// Idle for now
}

void AMenuPawn::OnClickedModeReadyReturn()
{
	CurrentState = 1;
}

void AMenuPawn::OnClickedEscapeMenuReturnToTitle()
{
	TargetSpline = MainMenuSpline;   // Redirect TargetSpline pointer
	CurrentState = 0;     // Switch state to character select
	InterpSpeed = 3;   // Menu pawn Location InterpSpeed
}

