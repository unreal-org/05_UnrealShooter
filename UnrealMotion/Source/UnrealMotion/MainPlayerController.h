// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UUIWidget;
class AStaticMeshActor;

/**
 * 
 */
UCLASS()
class UNREALMOTION_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// UI Widget Setters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> MainMenuTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> CharacterSelectTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> ModeSelectTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> ModeReadyTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> EscapeMenuTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UCameraShake> RecoilTarget;

	// UI Functions
	void OnClickedStart();
	void OnClickedReturnToTitle();
	void OnClickedCharacterNext();
	void OnClickedCharacterPrev();
	void OnClickedCharacterConfirm();
	void OnClickedModeSelectReturn();
	void OnClickedModeSelectPractice();
	void OnClickedModeSelectShowdown();
	void OnClickedModeReadyStart();
	void OnClickedModeReadyReturn();
	void OnClickedEscapeMenuReturnToTitle();

	// UI Accessors
	void EscapeMenuPressed();
	int32 GetMode();
	void ResetTargets();

	// Camera Shake
	void CameraShake();

protected:
	virtual void BeginPlay() override;

private:
	// UI Widget References
	UUIWidget* MainMenu;
	UUIWidget* CharacterSelect;
	UUIWidget* ModeSelect;
	UUIWidget* ModeReady;
	UUIWidget* EscapeMenu;

	// Practice Target Reference Array
	TArray<AStaticMeshActor*> PracticeTargets;
	TArray<FVector> PracticeTargetLocations;

	// Menu Pawn
	class AMenuPawn* MenuPawn = nullptr;
	
	// Character ID
	class AUnrealCharacter* Character = nullptr;
	FString TargetCharacter;

	// Escape Menu Open
	bool EscapeMenuOpen = false;

	// Mode
	int32 Mode = 0;
};
