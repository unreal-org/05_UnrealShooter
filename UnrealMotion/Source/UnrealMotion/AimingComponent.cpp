// Fill out your copyright notice in the Description page of Project Settings.


#include "AimingComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAimingComponent::UAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Gun to Set
void UAimingComponent::Initialize() // UGun* GunToSet
{
	// Gun = GunToSet;
}

// Aim at
void UAimingComponent::AimAt(FVector HitLocation)
{
	// if (!ensure(Gun)) { return; }

	// FVector OutLaunchVelocity = FVector(0, 0, 0);  
	// //FVector StartLocation = Gun->GetSocketLocation(FName("Projectile"));
	// bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
	// 		this,
	// 		OutLaunchVelocity,
	// 		StartLocation,
	// 		HitLocation,
	// 		LaunchSpeed,
	// 		false,  // default
	// 		0,  // default
	// 		0,  // default
	// 		ESuggestProjVelocityTraceOption::DoNotTrace  // must be present - presents bug "no solution found"
	// 	);

	// if (bHaveAimSolution)
	// {	
	// 	AimDirection = OutLaunchVelocity.GetSafeNormal();  // Get Unit Vector in direction of Launch
	// 	MoveBarrelTowards(AimDirection);
	// }

}


void UAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	// if (!ensure(Gun)) { return; }

	// work out difference between current barrel rotation and aimdirection
	//FRotator GunRotation = Gun + Arm ->GetForwardVector().Rotation();
	// FRotator AimAsRotator = AimDirection.Rotation();
	// FRotator DeltaRotatorPitch = AimAsRotator - GunRotation;
	// FRotator DeltaRotatorYaw = AimAsRotator - TurretRotation;
	
	// Barrel->Elevate(DeltaRotatorP.Pitch);
	// if (FMath::Abs(DeltaRotatorY.Yaw) < 180)  // always yaw shortest way
	// {
	// 	Turret->Rotate(DeltaRotatorY.Yaw);
	// }
	// else
	// {
	// 	Turret->Rotate(-DeltaRotatorY.Yaw);
	// }
}