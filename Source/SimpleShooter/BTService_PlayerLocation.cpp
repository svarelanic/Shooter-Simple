// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_PlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h" //For Getting Player Pawn
#include "GameFramework/Pawn.h"     //For getting ActorLocation from PlayerPawn

UBTService_PlayerLocation::UBTService_PlayerLocation()
{
    NodeName = TEXT("Update Player Location");
}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // Getting Player Pawn
    APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), // WorldContextObject
                                                        0);         // Player Index -> Zero Player is the Player controlled by user

    if (PlayerPawn == nullptr)
    {
        return;
    }

    // IMPLEMENTATION OF BEHAVIOR FROM ShooterAIController.cpp -> Seting LastKnownPlayerLocation key value as Vector
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(),      // The bb key set in the BT
                                                         PlayerPawn->GetActorLocation()); // The location of the ShooterCharacter
}
