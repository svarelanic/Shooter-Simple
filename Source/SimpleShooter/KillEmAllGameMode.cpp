// Fill out your copyright notice in the Description page of Project Settings.

#include "KillEmAllGameMode.h"
#include "EngineUtils.h"              //For TActorRange
#include "GameFramework/Controller.h" //To query a Controller to know wether or not is a Player Controller without Casting (Controller -> IsPlayerController())
#include "ShooterAIController.h"      //For the TActorRange<AShooterAIController> in the For loop

void AKillEmAllGameMode::PawnKilled(APawn *PawnKilled)
{
    Super::PawnKilled(PawnKilled);

    // Check if game is over
    // LOSING: If pawned killed is the Player you lose -> Check if pawn is a PlayerController: CAST
    APlayerController *PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController != nullptr)
    {
        // Player has died, Game has ended
        EndGame(false);
    }

    // PAWN KILLED WHEN ITS AN AI CONTROLLED PAWN -> ENDING GAME WITH WIN CONDITION WHEN ALL AI PAWNS KILLED:
    // For loop over ShooterAI in World:
    // Any Pawn Is not dead?
    // Early return
    // EndGame (if we did not early returned from the For loop)

    for (AShooterAIController *Controller : TActorRange<AShooterAIController>(GetWorld()))
    {
        // If any AIController is not dead -> we early Return because not all Pawn have been killed
        if (!Controller->IsDead())
        {
            return;
        }
    }
    // If we made it out of the For loop without early returning, all AI Pawns are Dead, we WIN:
    EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
    // Finding all controllers in our scene to call GameHasEnded on each of them
    // TActorRange -> A template that gives us all Controller Type actors in the world (gotten through GetWorld())
    for (AController *Controller : TActorRange<AController>(GetWorld())) // For loop over every Controller in the world and will be in this Controller called variable
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner; // If Player is Controller and Player is Winner, == is true so bIsWinner is True
                                                                              // If Player is Controller but not winner, == is false so bIsWinner is false
                                                                              // If Player is not controller and Player is not Winner, == is true so bIsWinner is also True
                                                                              // If Player is not Controller and Player is winner, == is false so bIsWinner is also False

        Controller->GameHasEnded(Controller->GetPawn(), // When Game ends camera remains on Player
                                 bIsWinner);
    }
}
