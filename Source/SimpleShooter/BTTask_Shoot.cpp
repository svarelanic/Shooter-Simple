// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Shoot.h"
#include "AIController.h"     //For the GetPawn()
#include "ShooterCharacter.h" //For casting into AShooterCharacter

UBTTask_Shoot::UBTTask_Shoot()
{
    // READABLE NAME FOR TASK ON THE EDITOR
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // Get hold of our Pawn -> Our ShooterCharacter.h has a Shoot() method we want to be calling
    // For this -> Get ahold of the AIController, from there we can get GetPawn() and cast that to our ShooterCharacter impementation

    // Check that our Controller is not null
    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AShooterCharacter *Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn()); // Cast into AShooterCharacter because GetPawn returns a Pawn type and Shoot is executed by a ShooterCharacter type

    // Check GetPawn returns null (if AI is not controlling a Pawn) or the Cast returns null (if the type controlled by the AI is not an AShooterCharacter)
    if (Character == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    Character->Shoot();

    return EBTNodeResult::Succeeded;
}
 