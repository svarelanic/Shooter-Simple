// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "KillEmAllGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AKillEmAllGameMode : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()

public:
	//Overriding PawnKilled from SimpleShooterGameModeBase
	virtual void PawnKilled(APawn *PawnKilled) override;

private:
	//To notify both Player Controllers and AI Controllers that the Game has ended
	void EndGame(bool bIsPlayerWinner); //false-> AI won, true->Player won
	
};
