// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_PlayerLocation.generated.h"

/**
 *
 */
UCLASS()
class SIMPLESHOOTER_API UBTService_PlayerLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	// CONSTRUCTOR
	UBTService_PlayerLocation();

protected:
	// Executing actions on a Tick Node every Tick this BTService is on an Active Path
	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
};
