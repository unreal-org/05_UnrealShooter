// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"
#include "UIWidget.h"
#include "MenuPawn.h"
#include "UnrealCharacter.h"
#include "Engine/StaticMeshActor.h"
#include "Containers/UnrealString.h"
#include "Engine/LocalPlayer.h"
#include "LAGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/UnrealString.h"
#include "GameFramework/GameState.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

    // Get MenuPawn Reference
    for (TActorIterator<AMenuPawn> It(GetWorld()); It; ++It)
	{
		AMenuPawn* Target = *It;
		if (Target && Target->GetName() == FString("MenuPawn_BP_C_0")) {
			MenuPawn = Target;
		}
	}

    if (UGameplayStatics::GetCurrentLevelName(MenuPawn->GetWorld(), true).Equals(FString("ShowdownMap"), ESearchCase::IgnoreCase))
    {
        Showdown = true;
        
        // Show Cursor
        bShowMouseCursor = true;

        ModeReady = CreateWidget<UUIWidget>(this, ModeReadyTarget);
        EscapeMenu = CreateWidget<UUIWidget>(this, EscapeMenuTarget);

        int32 playercount = UGameplayStatics::GetGameState(GetGameInstance()->GetWorld())->PlayerArray.Num();

        // UE_LOG(LogTemp, Warning, TEXT("player count : %i"), playercount)
        // Check GetGameState Player count to see if host or client
        if (playercount == 1) {
            // if Game State Players = 1, possess character 1
            TargetCharacter = FString("UnrealCharacter_BP_2");
        } else {
            // else possess character 2
            TargetCharacter = FString("UnrealCharacter_BP2_5");
        }

        if (ModeReady) { ModeReady->AddToViewport(); }
    }
    else {
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

        // Get Practice Target References
        FString PracticeTarget = FString("PracticeTarget_BP");
        const TCHAR* Substring = *PracticeTarget;
        for (TActorIterator<AStaticMeshActor> It(GetWorld()); It; ++It)
        {
            AStaticMeshActor* Target = *It;
            if (Target && Target->GetName().Contains(Substring, ESearchCase::IgnoreCase, ESearchDir::FromStart)) {
                PracticeTargets.Add(Target);
                PracticeTargetLocations.Add(Target->GetActorLocation());
            }
        }
    }
}

///////////////////////////////// UI Functions ////////////////////////////////////
void AMainPlayerController::OnClickedStart()
{
    if (!ensure(MenuPawn)) { return; }
    if (CharacterSelect) {
        MainMenu->RemoveFromViewport();
        CharacterSelect->AddToViewport();
        
        // Set MenuPawn's target spline to CharacterSelectSpline - Spline point 0
        MenuPawn->OnClickedStart();
    }
}

void AMainPlayerController::OnClickedReturnToTitle()
{
    if (!ensure(MenuPawn)) { return; }

    if (MainMenu) {
        CharacterSelect->RemoveFromViewport();
        MainMenu->AddToViewport();

        // Set MenuPawn's target spline to MainMenuSpline
        MenuPawn->OnClickedReturnToTitle();
    }
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

    if (ModeSelect) {
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

    if (CharacterSelect) {
        ModeSelect->RemoveFromViewport();
        CharacterSelect->AddToViewport();

        // Zoom out Camera
        MenuPawn->OnClickedModeSelectReturn();
    }
}

void AMainPlayerController::OnClickedModeSelectPractice()
{
    if (!ensure(MenuPawn)) { return; }

    if (ModeReady) {
        Mode = 1;

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
}

void AMainPlayerController::OnClickedModeSelectShowdown()
{
    if (!ensure(MenuPawn)) { return; }

    if (ModeReady) {
        Mode = 2;

        ModeSelect->RemoveFromViewport();
        // MenuPawn->OnClickedModeSelectShowdown();

        // Possess Character
        // for (TActorIterator<AUnrealCharacter> It(GetWorld()); It; ++It)
        // {
        //     class AUnrealCharacter* Target = *It;
        //     if (Target && Target->GetName() == TargetCharacter) {
        //         Character = Target;
        //     }
        // }

        // // Set Menu Pawn Targets
        // MenuPawn->SetCharacter(Character);

        // // Set Character Targets
        // Character->SetActorTickEnabled(true);
        // Character->SetCharacterGameLocation();
        // Character->SetCharacterRotation(FRotator(0, -90, 0));
        // Character->TargetState = 2;

        // ModeReady->AddToViewport();

        // Find Opponent or Host Session
        ULocalPlayer* Player = GetGameInstance()->GetFirstGamePlayer();
        Cast<ULAGameInstance>(Player->GetGameInstance())->Showdown();
    }
}

void AMainPlayerController::OnClickedModeReadyStart()
{
    if (Showdown == true) {
        for (TActorIterator<AUnrealCharacter> It(GetWorld()); It; ++It)
        {
            class AUnrealCharacter* Target = *It;
            if (Target && Target->GetName() == TargetCharacter) {
                Character = Target;
            }
        }

        ModeReady->RemoveFromViewport();

        Possess(Character);
        Character->SetShowdown(true);
        Character->SetCharacterState(2);

        bShowMouseCursor = false;
    }
    else {
        if (!ensure(MenuPawn) || !ensure(Character)) { return; }

        ModeReady->RemoveFromViewport();
        MenuPawn->OnClickedModeReadyStart();
        
        // Possess Character
        Possess(Character);
        Character->TargetState = 2;

        // Hide mouse cursor
        bShowMouseCursor = false;
    }
}

void AMainPlayerController::OnClickedModeReadyReturn()
{
    if (Showdown) {
        UGameplayStatics::OpenLevel(GetGameInstance()->GetWorld(), "UnrealMotionMap", true);

        // Destroy Session
    }
    else {
        if (!ensure(MenuPawn)) { return; }

        if (CharacterSelect) {
            Mode = 0;

            ModeReady->RemoveFromViewport();
            MenuPawn->OnClickedModeReadyReturn();
            CharacterSelect->AddToViewport();

            // Return Character to start location
            Character->SetCharacterStartLocation(MenuPawn->GetCharacterID());
            Character->SetCharacterRotation(FRotator(0, 0, 0));
            Character->TargetState = 0;
        }
    }
}

void AMainPlayerController::OnClickedEscapeMenuReturnToTitle()
{
    if (Showdown) { 
        UGameplayStatics::OpenLevel(GetGameInstance()->GetWorld(), "UnrealMotionMap", true);

        // Destroy Session
    }

    else {
        if (MainMenu) {
            Mode = 0;

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
    }
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
    if (RecoilTarget) {
        ClientPlayCameraShake(
            RecoilTarget,
            3,    // intensity
            ECameraAnimPlaySpace::CameraLocal,
            FRotator(0, 0, 0)
        );
    }
}

// Get Mode
int32 AMainPlayerController::GetMode()
{
    return Mode;
}

// Reset Targets
void AMainPlayerController::ResetTargets()
{
    if (Showdown) { return; }

    for (int32 i = 0; i < 8; i++)
	{
		PracticeTargets[i]->SetActorLocationAndRotation(PracticeTargetLocations[i], FRotator(0, 0, 0), false);
	}
}