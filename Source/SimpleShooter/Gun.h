// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

	// SHOOTING----------------
	// PULLING THE TRIGGER FUNCTION
	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// COMPONENTS FOR THE GUN:
	// Root Scene Component:
	UPROPERTY(VisibleAnywhere) // Components are visible anywhere since they're Pointers->We want to see and edit the properties of their class, but not edit the Pointer
	USceneComponent *Root;

	// Mesh Component
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent *Mesh; // Use UStaticMeshComponent if it does not have a skeleton or bones, it's static. In this case the Mesh was made as a Skeletal though

	// VARIABLES TO STORE VFX & SFX
	// Muzzle flash on Shot
	UPROPERTY(EditAnywhere)
	UParticleSystem *MuzzleFlash;
	// Muzzle Sound on Shot
	UPROPERTY(EditAnywhere)
	USoundBase *MuzzleSound;

	// Particle on impact
	UPROPERTY(EditAnywhere)
	UParticleSystem *HitParticle;
	// Sound on impact
	UPROPERTY(EditAnywhere)
	USoundBase *HitSound;

	// MAX RAY TRACING LINE RANGE
	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	// GUN DAMAGE
	UPROPERTY(EditAnywhere)
	float Damage = 10;

	// FUNCTION THAT GIVES US HIT RESULT AND SHOT DIRECTION FROM THE LINE TRACE. Since parameters are & (references) but not const: WE KNOW THOSE ARE OUT PARAMETERS
	bool GunTrace(FHitResult &Hit, FVector &ShotDirection);

	// FUNCTION THAT GIVES US THE OWNER CONTROLLER (to use in the TakeDamage() method in PullTtigger())
	AController *GetOwnerController() const;
};
