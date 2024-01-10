// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h" //To spawn Widget

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Creating Widget for HUD
    HUD = CreateWidget(this,      // Owner
                       HUDClass); // TSubclassOf UUserWidget class
    // Adding Widget to ViewPort to spawn it
    if (HUD != nullptr) // If we won
    {
        HUD->AddToViewport(); // from UserWidget.h
    }
}

void AShooterPlayerController::GameHasEnded(class AActor *EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    // REMOVING HUD ON GAME OVER
    HUD->RemoveFromParent();

    // SPAWNING UI WIDGET
    // Adding Widget to ViewPort to spawn it depending on winning or loosing
    if (bIsWinner) // If we won
    {
        // Creating Widget for Winning
        UUserWidget *WinScreen = CreateWidget(this,            // Owner
                                              WinScreenClass); // TSubclassOf UUserWidget class
        if (WinScreen != nullptr)
        {
            WinScreen->AddToViewport(); // from UserWidget.h
        }
    }
    else // if we lost
    {
        // Creating Widget for loosing
        UUserWidget *LoseScreen = CreateWidget(this,             // Owner
                                               LoseScreenClass); // TSubclassOf UUserWidget class
        if (LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport(); // from UserWidget.h
        }
    }

    // Restarting Game on a TIMER
    GetWorldTimerManager().SetTimer(RestartTimer,                     // Timer Handle
                                    this,                             // Object that we want the timer on
                                    &APlayerController::RestartLevel, // The method reference itself we want to run after timer
                                    RestartDelay);                    // The timer
}
