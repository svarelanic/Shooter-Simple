// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_PlayerLocationIfSeen.generated.h"

/**
 *
 */
UCLASS()
class SIMPLESHOOTER_API UBTService_PlayerLocationIfSeen : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	// CONSTRUCTOR
	UBTService_PlayerLocationIfSeen();

protected:
	// Executing actions on a Tick Node every Tick this BTService is on an Active Path
	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
};
