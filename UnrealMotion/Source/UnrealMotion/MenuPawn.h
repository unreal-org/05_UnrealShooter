// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MenuPawn.generated.h"

class AUnrealCharacter;

UCLASS()
class UNREALMOTION_API AMenuPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMenuPawn();

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
	
	// Character ID Access
	int32 GetCharacterID();

	// Set Character;
	void SetCharacter(AUnrealCharacter* TargetCharacter);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Character Reference
	AUnrealCharacter* CharacterReference = nullptr;

	// Spline References
	class USplineComponent* MainMenuSpline = nullptr;
	class USplineComponent* CharacterSelectSpline = nullptr;
	class USplineComponent* TargetSpline = nullptr;

	// Lerp & Interp
	void LerpAndInterp(float DeltaTime);
	float LITime = 0;
	float LIDuration = 0.5;
	float InterpSpeed;

	// Menu Pawn Variables
	float SplineLength;
	float SplineDistance;
	int32 CurrentState = 0;
	int32 CharacterID = 0;
	FVector TargetLocation;
	FRotator LookAtRotation;

};
