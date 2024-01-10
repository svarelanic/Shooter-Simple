// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue()
{
    // READABLE NAME FOR TASK ON THE EDITOR
    NodeName = TEXT("Clear Blackboard Value");
}

// EXECUTING BTTASK:
EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // Getting hold of our BB so we can clear the Key -> OwnerComp is a Reference (&) so we use . instead of -> to get the BBComponent
    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey()); // GetSelectedBlackboardKey returns an FName

    // ExecuteTask has a return value -> Of type EBTNodeResult::Scceded,Failed,Abort,InProgress
    return EBTNodeResult::Succeeded;
}
