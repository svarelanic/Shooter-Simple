// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 *
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Overriding GameHasEnded from PlayerController base Class
	virtual void GameHasEnded(class AActor *EndGameFocus = nullptr, bool bIsWinner = false) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// VARIABLE TO SET WHAT WIDGET CLASS WE SPAWN FOR HUD
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass; // Locks this to be of the UUserWidget Class

	// VARIABLE TO SET WHAT WIDGET CLASS WE SPAWN FOR LOSE SCREEN
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass; // Locks this to be of the UUserWidget Class
	// VARIABLE TO SET WHAT WIDGET CLASS WE SPAWN FOR WIN SCREEN
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass; // Locks this to be of the UUserWidget Class

	// TIMER -> To restart Game
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5;
	// TIMER HANDLE for the timer
	FTimerHandle RestartTimer;

	// HUD VARIABLE TO ACCESS IT BOTH IN BEGINPLAY (TO SPAWN IT) AND GAMEHASENDED (TO DESPAWN IT)
	UPROPERTY()
	UUserWidget *HUD;
};