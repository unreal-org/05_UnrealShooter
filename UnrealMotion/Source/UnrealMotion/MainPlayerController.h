// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UUIWidget;

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
	TSubclassOf<class UUserWidget> MainMenuTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> CharacterSelectTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> ModeSelectTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> ModeReadyTarget;

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

protected:
	virtual void BeginPlay() override;

private:
	// UI Widget References
	UUIWidget* MainMenu;
	UUIWidget* CharacterSelect;
	UUIWidget* ModeSelect;
	UUIWidget* ModeReady;

	// Menu Pawn
	class AMenuPawn* MenuPawn = nullptr;
	
	// Character ID
	class AUnrealCharacter* Character = nullptr;
	FString TargetCharacter;

	// Practice Area
	void ConstructPracticeArea();
};
