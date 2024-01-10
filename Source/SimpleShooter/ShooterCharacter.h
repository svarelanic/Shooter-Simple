// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

// FORWARD DECLARING GUN CLASS
class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// CHECK IF CHARACTER IS DEAD (to, for example, play Death Animations -> NEEDS TO BE ACCESSED THROUGH BP)
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	// BlueprintPure TO DETERMINE HEALTH PERCENTAGE IN THE HEALTH UNDER THE HUD
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input -> BIND INPUT HERE
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	// RECEIVING THE DAMAGE DEALT -> Overriding the TakeDamage method from Actor.h
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

	// FUNCTION TIED TO SHOOTING, WHERE WE CALL THE PullTrigger FUNCTION -> Public so we call it from a BTTask (so AI Shoots too!)
	void Shoot();

private:
	// MOVEMENT AND AIMING -------------
	// FUNCTION TIED TO MOVING FORWARD, NEEDED TO BIND MOVEMENT TO CHARACTER
	void MoveForward(float AxisValue);
	// FUNCTION TIED TO MOVING LEFT RIGHT, NEEDED TO BIND MOVEMENT TO CHARACTER
	void MoveRight(float AxisValue);
	// FUNCTION FOR LOOKING UP WITH CONTROLLER->MULTIPLYING BY DELTA TIME TO ELIMINATE FPS DEPENDANCY
	void LookUpRate(float AxisValue);
	// FUNCTION FOR LOOKING RIGHT WITH CONTROLLER->MULTIPLYING BY DELTA TIME TO ELIMINATE FPS DEPENDANCY
	void LookRightRate(float AxisValue);
	// ROTATION RATE VARIABLE TO MAKE AIMING INDEPENDANT FROM FPS (FOR CONRTOLLER USE) -> SPEED AT WHICH CAMERA MOVES
	UPROPERTY(EditAnywhere)
	float RotationRate = 10; // Same for UP/DOWN and LEFT/RIGHT

	// SPAWNING GUN ON RUNTIME -------
	// Setting to what class will the spawining be restricted to on the Editor:
	UPROPERTY(EditDefaultsOnly) // EditDefaultsOnly becuse we don't want it to be editable during RunTime, only before
	TSubclassOf<AGun> GunClass;
	// Where we store the instance after being spawned->in a Gun pointer
	UPROPERTY() // Not editable nor visible
	AGun *Gun;

	// HEALTH
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere)
	float Health; // On Begin Play -> Health = MaxHealth
};
