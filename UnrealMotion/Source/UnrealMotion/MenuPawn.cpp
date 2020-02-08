// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPawn.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"

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

	if (MainMenuSpline) {
		TargetLocation = MainMenuSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
		SplineLength = MainMenuSpline->GetSplineLength();
		TargetSpline = MainMenuSpline;
		SplineDistance = 0;
	}

	SetActorLocation(TargetLocation, false);
	CurrentState = 0;
	InterpSpeed = 3;
	
}

// Called every frame
void AMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
    {
        case 0: // Main Menu
			SplineDistance = UKismetMathLibrary::GenericPercent_FloatFloat(SplineDistance + DeltaTime * 25, SplineLength);
			if (!ensure(TargetSpline)) { return; }
			if (TargetSpline) { TargetLocation = TargetSpline->GetWorldLocationAtDistanceAlongSpline(SplineDistance); }
			LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(830, 870, 0));
            break;
		case 1: // Character Select
		if (!ensure(TargetSpline)) { return; }
			if (TargetSpline) { TargetLocation = TargetSpline->GetLocationAtSplinePoint(Character, ESplineCoordinateSpace::World); }
			LookAtRotation = FRotator(0, 180, 0);
			break;
		case 2: // Mode Select
			// if (TargetSpline) { TargetLocation = TargetSpline->GetLocationAtSplinePoint(Character, ESplineCoordinateSpace::World); }
			// LookAtRotation = FRotator(0, 180, 0);
			break;
    }

	LerpAndInterp(DeltaTime);
	
}

void AMenuPawn::LerpAndInterp(float DeltaTime)
{
	LITime = 0;
    if (LITime < LIDuration)
    {
        LITime += DeltaTime;
        FRotator MenuPawnRotation = FMath::Lerp(GetActorRotation(), LookAtRotation, LITime);
		FVector MenuPawnLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, InterpSpeed);

		SetActorRotation(MenuPawnRotation, ETeleportType::None);
		SetActorLocation(MenuPawnLocation, false);
    }
}

void AMenuPawn::OnClickedStart()
{
	CurrentState = 1;
	TargetSpline = CharacterSelectSpline;
	Character = 0;
	InterpSpeed = 10;
}

void AMenuPawn::OnClickedReturnToTitle()
{
	CurrentState = 0;
	TargetSpline = MainMenuSpline;
	Character = 0;
	InterpSpeed = 3;
}

void AMenuPawn::OnClickedCharacterNext()
{
	Character = abs(UKismetMathLibrary::GenericPercent_FloatFloat(Character + 1, 4));
}

void AMenuPawn::OnClickedCharacterPrev()
{
	if (Character == 0) { Character = 3; }
	else { Character = abs(UKismetMathLibrary::GenericPercent_FloatFloat(Character - 1, 4)); }
}

void AMenuPawn::OnClickedCharacterConfirm()
{
	// Zoom in Camera
	// Possess Character
	// Move Character to center stage
}

void AMenuPawn::OnClickedModeSelectReturn()
{
	// Zoom out Camera
}

void AMenuPawn::OnClickedModeSelectPractice()
{
	// Generate Training Area
}

void AMenuPawn::OnClickedModeSelectShowdown()
{
	// Find opponent via network play
}

