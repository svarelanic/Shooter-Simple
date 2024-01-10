// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 *
 */
UCLASS()
class SIMPLESHOOTER_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Query AI Controlled enemies if they are dead or not
	bool IsDead() const; //Only counts, hence const

private:
/*
	//Acceptence Radius (Radius to which AI moves to from the Player)
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 200;
*/

	//BEHAVIOR TREE--------------------------------
	//REFERENCING IN BP WHAT BEHAVIOR TREE WE WANT IT TO RUN
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;  

};
