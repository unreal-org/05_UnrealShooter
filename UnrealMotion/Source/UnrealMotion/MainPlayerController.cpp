// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"
#include "UIWidget.h"
#include "MenuPawn.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

    if (!ensure(MainMenuTarget)) { return; }
    if (!ensure(CharacterSelectTarget)) { return; }
    if (!ensure(ModeSelectTarget)) { return; }
	
    MainMenu = CreateWidget<UUIWidget>(this, MainMenuTarget);
    CharacterSelect = CreateWidget<UUIWidget>(this, CharacterSelectTarget);
    ModeSelect = CreateWidget<UUIWidget>(this, ModeSelectTarget);

    if (MainMenu) { MainMenu->AddToViewport(); }

    bShowMouseCursor = true;

    for (TActorIterator<AMenuPawn> It(GetWorld()); It; ++It)
	{
		AMenuPawn* Target = *It;
		if (Target && Target->GetName() == FString("MenuPawn_BP_C_0")) {
			MenuPawn = Target;
		}
	}
}

void AMainPlayerController::OnClickedStart()
{
    MainMenu->RemoveFromViewport();
    CharacterSelect->AddToViewport();
    
    // Set MenuPawn's target spline to CharacterSelectSpline - Spline point 0
    MenuPawn->OnClickedStart();
}

void AMainPlayerController::OnClickedReturnToTitle()
{
    CharacterSelect->RemoveFromViewport();
    MainMenu->AddToViewport();

    // Set MenuPawn's target spline to MainMenuSpline
    MenuPawn->OnClickedReturnToTitle();
}

void AMainPlayerController::OnClickedCharacterNext()
{
    // Set MenuPawn's target spline point to next character
    MenuPawn->OnClickedCharacterNext();
}

void AMainPlayerController::OnClickedCharacterPrev()
{
    // Set MenuPawn's target spline point to prev character
    MenuPawn->OnClickedCharacterPrev();
}

void AMainPlayerController::OnClickedCharacterConfirm()
{
    CharacterSelect->RemoveFromViewport();
    ModeSelect->AddToViewport();

    // Possess Chosen Character & Pan Camera & Play Chosen Character Animation
    MenuPawn->OnClickedCharacterConfirm();
}

void AMainPlayerController::OnClickedModeSelectReturn()
{
    ModeSelect->RemoveFromViewport();
    CharacterSelect->AddToViewport();

    // DePossess Chosen Character & DePan Camera
    MenuPawn->OnClickedModeSelectReturn();
}

void AMainPlayerController::OnClickedModeSelectPractice()
{
    // Generate Target Practice Area
    MenuPawn->OnClickedModeSelectPractice();
}

void AMainPlayerController::OnClickedModeSelectShowdown()
{
    // Generate Western Town Area & Find opponent for Network Play
    MenuPawn->OnClickedModeSelectShowdown();
}