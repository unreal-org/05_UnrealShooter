// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MenuPawn.generated.h"

UCLASS()
class UNREALMOTION_API AMenuPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMenuPawn();

	void OnClickedStart();
	void OnClickedReturnToTitle();
	void OnClickedCharacterNext();
	void OnClickedCharacterPrev();
	void OnClickedCharacterConfirm();
	void OnClickedModeSelectReturn();
	void OnClickedModeSelectPractice();
	void OnClickedModeSelectShowdown();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	class USplineComponent* MainMenuSpline = nullptr;
	class USplineComponent* CharacterSelectSpline = nullptr;
	class USplineComponent* TargetSpline = nullptr;

	void LerpAndInterp(float DeltaTime);
	float LITime = 0;
	float LIDuration = 0.5;
	float InterpSpeed;

	FVector TargetLocation;
	float SplineLength;
	float SplineDistance;
	int32 CurrentState = 0;
	int32 Character = 0;
	FRotator LookAtRotation;

};
