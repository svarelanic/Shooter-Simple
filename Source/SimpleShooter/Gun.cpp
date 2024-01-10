// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// CREATING ROOT SCENE COMPONENT & SETTING IT AS CLASS ROOT
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root")); // Create a USceneComponent type component named Root
	SetRootComponent(Root);										  // Setting said component as class Root

	// CREATING MESH COMPONENT & ATTACHING TO ROOT SCENE COMPONENT
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh")); // Creating Mesh Component
	Mesh->SetupAttachment(Root);										 // Attaching it to Root Scene Component
}

// SHOOITN ARCHITECTURE: Pulling the Trigger
void AGun::PullTrigger()
{
	// MUZZLE FLASH PARTICLE EFFECT
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash,				   // The particle effect -> it is assigned to this variable on BP
										   Mesh,					   // The Component we attach it to
										   TEXT("MuzzleFlashSocket")); // The SOCKET we attach it to

	// MUZZLE FLASH SOUND
	UGameplayStatics::SpawnSoundAttached(MuzzleSound,				 // The Sound effect -> it is assigned to this variable on BP
										 Mesh,						 // The Component we attach it to
										 TEXT("MuzzleFlashSocket")); // The SOCKET we attach it to

	// PASSING IN THE HIT RESULT AND SHOT DIRECTION FROM GunTrace() METHOD:
	// Declaring the out parameters:
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection); // Gun Trace will add value to Hit and ShotDirection
	if (bSuccess)
	{
		// ON HIT PARTICLE EFFECT
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),				// World Context
												 HitParticle,				// Particle System
												 Hit.Location,				// Location
												 ShotDirection.Rotation()); // Rotation

		// ON HIT SOUND EFFECT
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(),	  // World Context
											   HitSound,	  // Sound Effect
											   Hit.Location); // Location

		// DEALING DAMAGE
		// Getting ahold of the hit actor:
		AActor *HitActor = Hit.GetActor();
		// Checking HitActor is valid
		if (HitActor != nullptr)
		{
			// Constructing Damage Event:
			FPointDamageEvent DamageEvent(Damage,		 // Damage caused
										  Hit,			 // InHit information
										  ShotDirection, // Shot direction
										  nullptr);		 // Damage Subclass -> Since we are not using a special damage type, this is null

			// Call the TakeDamage Function:
			AController *OwnerController = GetOwnerController(); // Get the OwnerController first for TakeDamage

			HitActor->TakeDamage(Damage,		  // Damage amount
								 DamageEvent,	  // Damage event struct we construced above
								 OwnerController, // Gun's Controller we got ahold of above
								 this);			  // The actor doing the damage, the Gun itself
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGun::GunTrace(FHitResult &Hit, FVector &ShotDirection)
{
	// GETTING PLAYER VIEWPOINT -> FROM CONTROLLER------------------------------------------------------------
	// a) to c) SET OWNER CONTROLLER FROM GetOwnerController() METHOD
	AController *OwnerController = GetOwnerController();
	// d) Check Controller isnt null
	if (OwnerController == nullptr)
	{
		return false;
	}
	// e)Get Player ViewPoint from Controller: -> WE MUST FIRST DECLARE ROTATION AND LOCATION SO THAT GetPlayerViewPoint FUNCTION GIVES THEM VALUE!
	// OUT PARAMETERS:
	FVector Location;
	FRotator Rotation;
	// GETTING THE VIEWPOINT INFORMATION INTO THE OUT PARAMETERS: -> Because the out parameters have just been declare, we know they are out parameters
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	// SETTING SHOT DIRECTION
	ShotDirection = -Rotation.Vector(); // Vector to get the shot direction, which is the oposite of the impact rotation (to see where the shot came from, the oposite to where it was going)

	// CREATING LINE TRACE------------------------------------------------------------------------------------
	// a) Start Location: Already have it, its the Location from the ViewPoint
	// b) End Location:
	FVector End = Location + Rotation.Vector() * MaxRange; // Max Range is a default UPROPERTY value created on Gun.h for this purpose
	// c) Line Trace by Channel / Using PARAMS to ignore acotrs in the Line Trace
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);		// Ignore the gun, this
	Params.AddIgnoredActor(GetOwner()); // Ignore the Gun's Owner Pawn

	return GetWorld()->LineTraceSingleByChannel(Hit, // This Hit will be passed as the Out Parameter
												Location,
												End,
												ECollisionChannel::ECC_GameTraceChannel1, // Default Channel Name given tu Bullet Channel. Obtained from Project files -> Config -> DefaultEngine.ini
												Params);								  // The Params that allow us to ignore the gun and owner pawn
}

AController *AGun::GetOwnerController() const
{
	// GETTING PLAYER VIEWPOINT -> FROM CONTROLLER------------------------------------------------------------
	// a) Get ahold of the Controller Pawn (we cast since it doesnt know it is an ACharacterShooter class. We can cast to an APawn to be as non specific as possible):
	APawn *OwnerPawn = Cast<APawn>(GetOwner());
	// b)Check OwnerPawn is not null, if it is, we return early from our trigger function
	if (OwnerPawn == nullptr)
	{
		return nullptr; // Return the type of the function
	}
	// c) Get Controller itself from the Pawn
	return OwnerPawn->GetController();
}
