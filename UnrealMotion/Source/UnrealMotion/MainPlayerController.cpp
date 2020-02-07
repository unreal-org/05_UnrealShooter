// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UIWidget.h"

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
}

void AMainPlayerController::OnClickedStart()
{
    MainMenu->RemoveFromViewport();
    CharacterSelect->AddToViewport();
    
    // Set MenuPawn's target spline to CharacterSelectSpline - Spline point 0
}

void AMainPlayerController::OnClickedReturnToTitle()
{
    CharacterSelect->RemoveFromViewport();
    MainMenu->AddToViewport();

    // Set MenuPawn's target spline to MainMenuSpline
}

void AMainPlayerController::OnClickedCharacterNext()
{
    // Set MenuPawn's target spline point to +1
}

void AMainPlayerController::OnClickedCharacterPrev()
{
    // Set MenuPawn's target spline point to -1
}

void AMainPlayerController::OnClickedCharacterConfirm()
{
    CharacterSelect->RemoveFromViewport();
    ModeSelect->AddToViewport();

    // Possess Chosen Character & Pan Camera & Play Chosen Character Animation
}

void AMainPlayerController::OnClickedModeSelectReturn()
{
    ModeSelect->RemoveFromViewport();
    CharacterSelect->AddToViewport();

    // DePossess Chosen Character & DePan Camera
}

void AMainPlayerController::OnClickedModeSelectPractice()
{
    // Generate Target Practice Area
}

void AMainPlayerController::OnClickedModeSelectShowdown()
{
    // Generate Western Town Area & Find opponent for Network Play
}