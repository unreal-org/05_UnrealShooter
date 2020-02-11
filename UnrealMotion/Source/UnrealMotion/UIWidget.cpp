// Fill out your copyright notice in the Description page of Project Settings.


#include "UIWidget.h"
#include "Engine/World.h"
#include "MainPlayerController.h"

////////////////////////////////// UI Functions ///////////////////////////////////
void UUIWidget::OnClickedStart()
{
    AMainPlayerController* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();

    if (!ensure(MainPlayerController)) { return; }
    MainPlayerController->OnClickedStart();
}

void UUIWidget::OnClickedReturnToTitle()
{
    AMainPlayerController* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();

    if (!ensure(MainPlayerController)) { return; }
    MainPlayerController->OnClickedReturnToTitle();
}

void UUIWidget::OnClickedCharacterNext()
{
    AMainPlayerController* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();

    if (!ensure(MainPlayerController)) { return; }
    MainPlayerController->OnClickedCharacterNext();
}

void UUIWidget::OnClickedCharacterPrev()
{
    AMainPlayerController* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();

    if (!ensure(MainPlayerController)) { return; }
    MainPlayerController->OnClickedCharacterPrev();
}

void UUIWidget::OnClickedCharacterConfirm()
{
    AMainPlayerController* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();

    if (!ensure(MainPlayerController)) { return; }
    MainPlayerController->OnClickedCharacterConfirm();
}

void UUIWidget::OnClickedModeSelectReturn()
{
    AMainPlayerController* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();

    if (!ensure(MainPlayerController)) { return; }
    MainPlayerController->OnClickedModeSelectReturn();
}

void UUIWidget::OnClickedModeSelectPractice()
{
    AMainPlayerController* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();

    if (!ensure(MainPlayerController)) { return; }
    MainPlayerController->OnClickedModeSelectPractice();
}

void UUIWidget::OnClickedModeSelectShowdown()
{
    AMainPlayerController* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();

    if (!ensure(MainPlayerController)) { return; }
    MainPlayerController->OnClickedModeSelectShowdown();
}

void UUIWidget::OnClickedModeReadyStart()
{
    AMainPlayerController* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();

    if (!ensure(MainPlayerController)) { return; }
    MainPlayerController->OnClickedModeReadyStart();
}

void UUIWidget::OnClickedModeReadyReturn()
{
    AMainPlayerController* MainPlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();

    if (!ensure(MainPlayerController)) { return; }
    MainPlayerController->OnClickedModeReadyReturn();
}