// Fill out your copyright notice in the Description page of Project Settings.


#include "UIWidget.h"
#include "Engine/World.h"
#include "MainPlayerController.h"

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