// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SimpleShooterGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class SIMPLESHOOTER_API ASimpleShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// PawnKilled function that we will OVERRIDE in our Custom Game Mode , hence de VIRTUAL
	virtual void PawnKilled(APawn *PawnKilled);
};
