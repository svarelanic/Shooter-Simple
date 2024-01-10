// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_PlayerLocationIfSeen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h" //For Getting Player Pawn
#include "GameFramework/Pawn.h"     //For getting ActorLocation from PlayerPawn
#include "AIController.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
    NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // Getting Player Pawn
    APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), // WorldContextObject
                                                        0);         // Player Index -> Zero Player is the Player controlled by user

    if (PlayerPawn == nullptr)
    {
        return;
    }

    // Check that we have an AI Controller
    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return;
    }

    // Check for visibility
    if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn)) // LineOfSightTo -> built in method from AIController.h, which inherits ShooterAIController
    {
        // If we have line of sight, we set the PlayerLocation (as the Player)
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), // Name shown in the BB Key
                                                             PlayerPawn);                // The Player PAWN said Key will represent
    }
    else // Out of line of sight
    {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }
}

