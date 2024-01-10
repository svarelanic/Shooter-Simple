// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Setting initial Health = MaxHealth
	Health = MaxHealth;

	// SPAWINING A GUN ACTOR ON RUNTIME FROM OUR ShooterCharacter CLASS
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);

	// HIDING STATIC GUN MESH FROM SHOOTERCHARACTER MESH -> SO WE CAN ATTACH THE GUN ACTOR AS CHILD
	GetMesh()->HideBoneByName(TEXT("weapon_r"),		  // Gun's bone's Name
							  EPhysBodyOp::PBO_None); // Option for physics we decide to leave as none by default

	// GETTING THE GUN ACTOR INTO THE CHARACTER:
	//  ATTACHING THE GUN ACTOR TO THE WeaponSocket ON THE MESH'S SKELETON
	Gun->AttachToComponent(GetMesh(),										 // A USceneComponent* Parent, to what we are attaching to
						   FAttachmentTransformRules::KeepRelativeTransform, // We keep the Gun Relative to the Mesh, so it moves with it and not with the world
						   TEXT("WeaponSocket"));							 // Socket we are attaching the gun into
	// SETTING THE ShooterCharacter CLASS AS THE OWNER OF THE GUN ACTOR -> Comes into play in multiplayer and damage calcs
	Gun->SetOwner(this); // Set the ShooterCharacter Class as the owner of the Gun Actor.
}

bool AShooterCharacter::IsDead() const
{
	/*if (Health == 0)
	{
		return true;
	}
	return false;*/

	return Health <= 0; // Same as code above
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth; // Percentage of Health under Total Health
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// SETTING UP PLAYER INPUT FOR MOVING/AIMING
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// BINDING FORWARD/BACKWARD INPUT
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),			   // The input we'll bind -> The AxisValue we'll use
								   this,						   // The actor we'll call the function on
								   &AShooterCharacter::MoveForward // The pointer to the function we'll call -> must create function
	);

	// BINDING LOOKING UP AND DOWN INPUT
	PlayerInputComponent->BindAxis(TEXT("LookUp"),				   // The input we'll bind -> The AxisValue we'll use
								   this,						   // The actor we'll call the function on
								   &APawn::AddControllerPitchInput // The pointer to the function we'll call -> call it directly from APawn
	);

	// BINDING MOVING LEFT/RIGHT INPUT
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),			 // The input we'll bind -> The AxisValue we'll use
								   this,						 // The actor we'll call the function on
								   &AShooterCharacter::MoveRight // The pointer to the function we'll call -> must create function
	);

	// BINDING LOOKING LEFT AND RIGHT INPUT
	PlayerInputComponent->BindAxis(TEXT("LookRight"),			 // The input we'll bind -> The AxisValue we'll use
								   this,						 // The actor we'll call the function on
								   &APawn::AddControllerYawInput // The pointer to the function we'll call -> call it directly from APawn
	);

	// BINDING JUMP ACTION INPUT -> USE BindAction
	PlayerInputComponent->BindAction(TEXT("Jump"),			  // The input we'll bind -> The AxisValue we'll use
									 EInputEvent::IE_Pressed, // THE EVENT HAPPENS WHEN I PRESS (that's the function of EInputEvent)
									 this,					  // The actor we'll call the function on
									 &ACharacter::Jump		  // IMPORTANT: Jump function comes already defined in Character Class
	);

	// BINDING LOOKING UP AND DOWN INPUT WITH CONTROLLER
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"),			  // The input we'll bind -> The AxisValue we'll use
								   this,						  // The actor we'll call the function on
								   &AShooterCharacter::LookUpRate // The pointer to the function we'll call -> Where we multiply for DeltaTime
	);

	// BINDING LOOKING RIGHT AND DOWN INPUT WITH CONTROLLER
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"),			 // The input we'll bind -> The AxisValue we'll use
								   this,							 // The actor we'll call the function on
								   &AShooterCharacter::LookRightRate // The pointer to the function we'll call -> Where we multiply for DeltaTime
	);

	// BINDING SHOOTING ACTION INPUT -> USE BindAction
	PlayerInputComponent->BindAction(TEXT("Shoot"),			   // The input we'll bind -> The AxisValue we'll use
									 EInputEvent::IE_Pressed,  // THE EVENT HAPPENS WHEN I PRESS (that's the function of EInputEvent)
									 this,					   // The actor we'll call the function on
									 &AShooterCharacter::Shoot // We define it ourselves, here we call the PullTrigger function
	);
}

// Overriding TakeDamage from actor Actor.h to RECEIVE DAMAGE DEALT BY GUN
float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	// Calling the parent implementation of TakeDamage: -> Has a Return type: the damage applied
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	// Damaging Health:
	DamageToApply = FMath::Min(Health, DamageToApply); // When DamageToApply > Health -> DamageToApply=Health (so that we dont go negative)
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Health Left %f"), Health);

	// AFTER DEATH, WE WANT CHARACTERS TO STOP SHOOTING, NOT HAVE CAPSULE COLLISION, CANT MOVE:
	if (IsDead())
	{
		// Getting hold of our Game Mode and calling PawnKilled
		ASimpleShooterGameModeBase *GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this); // Pawn killed is this
		}

		DetachFromControllerPendingDestroy();										// Detach controller, won't shoot or move anymore
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disables Capsule Collision
	}

	return DamageToApply; // RETURN VALUE OF TakeDamage()
}

// MOVE FORWARD/BACKWARDS FUNCTION USED IN BindAxis
void AShooterCharacter::MoveForward(float AxisValue)
{
	// PASS THE AXIS VALUE ON TO THIS FUNCTION, WHICH WE CALL ON THE BIND AXIS FUNCTION ABOVE
	AddMovementInput(GetActorForwardVector() * AxisValue); // We get to forward vector and mulitply it by axis value, if 1 move forward if -1 move backwards
}
// MOVE RIGHT/LEFT FUNCTION USED IN BindAxis
void AShooterCharacter::MoveRight(float AxisValue)
{
	// PASS THE AXIS VALUE ON TO THIS FUNCTION, WHICH WE CALL ON THE BIND AXIS FUNCTION ABOVE
	AddMovementInput(GetActorRightVector() * AxisValue); // We get to forward vector and mulitply it by axis value, if 1 move forward if -1 move backwards
}

// LOOK UP RATE CALCULATIONS TO LOOK UP WITH CONTROLLER
void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds()); // Multiply AxisValue (which is 1) by the rotation (which we define, its how fast it'll go) and by the time it takes between Frames (in seconds) to eliminate the dependancy of FPS
}
// LOOK RIGHT RATE CALCULATIONS TO LOOK UP WITH CONTROLLER
void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds()); // Multiply AxisValue (which is 1) by the rotation (which we define, its how fast it'll go) and by the time it takes between Frames (in seconds) to eliminate the dependancy of FPS
}

// SHOOT FUNCTION FOR KEY BINDING, WHERE WE EXECUTE PullTrigger FUNCTION
void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}
