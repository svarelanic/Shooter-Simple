// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h" //For Getting Player Pawn
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h" //For casting into ShooterCharacter in IsDead()

class UBehaviorTree; // Forward declaration of class

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    // USING BEHAVIOR TREE
    // Check we actually have a BT:
    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior); // Running the BT

        // SETTING AI CONTROLLED PAWN START LOCATION AS A BB KEY
        // a) Getting ahold of controlled Pawn -> GetPawn()
        // b) Get the BBComponent and, from it, SetVector to add the Start Location to Key
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"),          // Name shown in the BB Key
                                                   GetPawn()->GetActorLocation()); // The Controlled Pawn Location said Key will represent
    }
}

void AShooterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool AShooterAIController::IsDead() const
{
    // Get the Pawn and cast it to make sure it is a ShooterCharacter
    AShooterCharacter *ControlledCharacter = Cast<AShooterCharacter>(GetPawn());

    if (ControlledCharacter != nullptr)
    {
        // We check if it's deado or not, calling the function from AShooterCharacter.h
        return ControlledCharacter->IsDead();
    }

    // If Controlled Character == nullptr, we dont have an AI Controlled Character which means is dead, since on death we remove controll
    return true; // true for its dead
}