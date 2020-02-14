// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALMOTION_API UUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// UI Functions
	UFUNCTION(BlueprintCallable, Category = "UI Functions")
	void OnClickedStart();

	UFUNCTION(BlueprintCallable, Category = "UI Functions")
	void OnClickedReturnToTitle();
	
	UFUNCTION(BlueprintCallable, Category = "UI Functions")
	void OnClickedCharacterNext();
	
	UFUNCTION(BlueprintCallable, Category = "UI Functions")
	void OnClickedCharacterPrev();
	
	UFUNCTION(BlueprintCallable, Category = "UI Functions")
	void OnClickedCharacterConfirm();
	
	UFUNCTION(BlueprintCallable, Category = "UI Functions")
	void OnClickedModeSelectReturn();
	
	UFUNCTION(BlueprintCallable, Category = "UI Functions")
	void OnClickedModeSelectPractice();
	
	UFUNCTION(BlueprintCallable, Category = "UI Functions")
	void OnClickedModeSelectShowdown();
	
	UFUNCTION(BlueprintCallable, Category = "UI Functions")
	void OnClickedModeReadyStart();
	
	UFUNCTION(BlueprintCallable, Category = "UI Functions")
	void OnClickedModeReadyReturn();

	UFUNCTION(BlueprintCallable, Category = "UI Functions")
	void OnClickedEscapeMenuReturnToTitle();

};
