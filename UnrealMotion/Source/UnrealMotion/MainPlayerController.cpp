// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"
#include "UIWidget.h"
#include "MenuPawn.h"
#include "UnrealCharacter.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

    if (!ensure(MainMenuTarget)) { return; }
    if (!ensure(CharacterSelectTarget)) { return; }
    if (!ensure(ModeSelectTarget)) { return; }
	
    // Set UI Widget references
    MainMenu = CreateWidget<UUIWidget>(this, MainMenuTarget);
    CharacterSelect = CreateWidget<UUIWidget>(this, CharacterSelectTarget);
    ModeSelect = CreateWidget<UUIWidget>(this, ModeSelectTarget);
    ModeReady = CreateWidget<UUIWidget>(this, ModeReadyTarget);
    EscapeMenu = CreateWidget<UUIWidget>(this, EscapeMenuTarget);

    // Add MainMenu to viewport
    if (MainMenu) { MainMenu->AddToViewport(); }

    // Show Cursor
    bShowMouseCursor = true;

    // Get MenuPawn Reference
    for (TActorIterator<AMenuPawn> It(GetWorld()); It; ++It)
	{
		AMenuPawn* Target = *It;
		if (Target && Target->GetName() == FString("MenuPawn_BP_C_0")) {
			MenuPawn = Target;
		}
	}

}

///////////////////////////////// UI Functions ////////////////////////////////////
void AMainPlayerController::OnClickedStart()
{
    if (!ensure(MenuPawn)) { return; }

    MainMenu->RemoveFromViewport();
    CharacterSelect->AddToViewport();
    
    // Set MenuPawn's target spline to CharacterSelectSpline - Spline point 0
    MenuPawn->OnClickedStart();
}

void AMainPlayerController::OnClickedReturnToTitle()
{
    if (!ensure(MenuPawn)) { return; }

    CharacterSelect->RemoveFromViewport();
    MainMenu->AddToViewport();

    // Set MenuPawn's target spline to MainMenuSpline
    MenuPawn->OnClickedReturnToTitle();
}

void AMainPlayerController::OnClickedCharacterNext()
{
    if (!ensure(MenuPawn)) { return; }

    // Set MenuPawn's target spline point to next character
    MenuPawn->OnClickedCharacterNext();
}

void AMainPlayerController::OnClickedCharacterPrev()
{
    if (!ensure(MenuPawn)) { return; }
    
    // Set MenuPawn's target spline point to prev character
    MenuPawn->OnClickedCharacterPrev();
}

void AMainPlayerController::OnClickedCharacterConfirm()
{
    if (!ensure(MenuPawn)) { return; }

    if (MenuPawn) {
        CharacterSelect->RemoveFromViewport();
        ModeSelect->AddToViewport();

        // Zoom in Camera & Play Chosen Character Animation
        MenuPawn->OnClickedCharacterConfirm();

        // Determine Character ID
        switch(MenuPawn->GetCharacterID())
        {
            case 0: // Character 0
                TargetCharacter = FString("UnrealCharacter_BP_2");
                break;
            case 1: // Character 1
                TargetCharacter = FString("UnrealCharacter_BP2_2");
                break;
            case 2: // Character 2
                TargetCharacter = FString("UnrealCharacter_BP3_5");
                break;
            case 3: // Character 3
                TargetCharacter = FString("UnrealCharacter_BP4_8");
                break;
        }
    }
}

void AMainPlayerController::OnClickedModeSelectReturn()
{
    if (!ensure(MenuPawn)) { return; }

    ModeSelect->RemoveFromViewport();
    CharacterSelect->AddToViewport();

    // Zoom out Camera
    MenuPawn->OnClickedModeSelectReturn();
}

void AMainPlayerController::OnClickedModeSelectPractice()
{
    if (!ensure(MenuPawn)) { return; }

    ModeSelect->RemoveFromViewport();
    MenuPawn->OnClickedModeSelectPractice();

    // Possess Character
	for (TActorIterator<AUnrealCharacter> It(GetWorld()); It; ++It)
	{
		class AUnrealCharacter* Target = *It;
		if (Target && Target->GetName() == TargetCharacter) {
			Character = Target;
		}
	}

    // Set Menu Pawn Targets
    MenuPawn->SetCharacter(Character);

    // Set Character Targets
    Character->SetActorTickEnabled(true);
    Character->SetCharacterGameLocation();
    Character->SetCharacterRotation(FRotator(0, 90, 0));
    Character->TargetState = 2;

    ModeReady->AddToViewport();
}

void AMainPlayerController::OnClickedModeSelectShowdown()
{
    if (!ensure(MenuPawn)) { return; }

    ModeSelect->RemoveFromViewport();
    MenuPawn->OnClickedModeSelectShowdown();

    // Possess Character
	for (TActorIterator<AUnrealCharacter> It(GetWorld()); It; ++It)
	{
		class AUnrealCharacter* Target = *It;
		if (Target && Target->GetName() == TargetCharacter) {
			Character = Target;
		}
	}

    // Set Menu Pawn Targets
    MenuPawn->SetCharacter(Character);

    // Set Character Targets
    Character->SetActorTickEnabled(true);
    Character->SetCharacterGameLocation();
    Character->SetCharacterRotation(FRotator(0, -90, 0));
    Character->TargetState = 2;

    ModeReady->AddToViewport();

    // Find Opponent
}

void AMainPlayerController::OnClickedModeReadyStart()
{
    if (!ensure(MenuPawn) || !ensure(Character)) { return; }

    ModeReady->RemoveFromViewport();
    MenuPawn->OnClickedModeReadyStart();
    
    // Possess Character
    Possess(Character);
    Character->TargetState = 2;

    // Hide mouse cursor
    bShowMouseCursor = false;
}

void AMainPlayerController::OnClickedModeReadyReturn()
{
    if (!ensure(MenuPawn)) { return; }

    ModeReady->RemoveFromViewport();
    MenuPawn->OnClickedModeReadyReturn();
    CharacterSelect->AddToViewport();

    // Return Character to start location
    Character->SetCharacterStartLocation(MenuPawn->GetCharacterID());
    Character->SetCharacterRotation(FRotator(0, 0, 0));
    Character->TargetState = 0;
}

void AMainPlayerController::OnClickedEscapeMenuReturnToTitle()
{
    EscapeMenu->RemoveFromViewport();
    MainMenu->AddToViewport();

    MenuPawn->OnClickedEscapeMenuReturnToTitle();
    UnPossess();
    Possess(MenuPawn);

    Character->SpawnDefaultController();
    Character->SetCharacterStartLocation(MenuPawn->GetCharacterID());
    Character->SetCharacterRotation(FRotator(0, 0, 0));
    Character->TargetState = 0;
}

// UI Accessors
void AMainPlayerController::EscapeMenuPressed()
{
    if (!ensure(EscapeMenu)) { return; }

    if (EscapeMenuOpen == false) {
        EscapeMenu->AddToViewport();
        EscapeMenuOpen = true;
        bShowMouseCursor = true;
    } else {
        EscapeMenu->RemoveFromViewport();
        EscapeMenuOpen = false;
        bShowMouseCursor = false;
    }
}

// Camera Shake
void AMainPlayerController::CameraShake()
{
    ClientPlayCameraShake(
        RecoilTarget,
        3,    // intensity
        ECameraAnimPlaySpace::CameraLocal,
        FRotator(0, 0, 0)
    );
}