// Fill out your copyright notice in the Description page of Project Settings.


#include "AimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Bullet.h"
#include "Engine/World.h"

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

// Ready Aim
FVector UAimingComponent::ReadyAim(FVector HitLocation)
{
	// Get Aim Direction
	FVector OutLaunchVelocity = FVector(0, 0, 0);  
	FVector StartLocation = GetOwner()->FindComponentByClass<UStaticMeshComponent>()->GetSocketLocation(FName("Projectile"));

	bool HaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
			this,
			OutLaunchVelocity,
			StartLocation,
			HitLocation,
			LaunchSpeed,
			false,  
			0,  
			0, 
			ESuggestProjVelocityTraceOption::DoNotTrace 
		);


	// Get & Set Aim Rotation
	if(HaveAimSolution)
	{	
		FVector AimDirection = OutLaunchVelocity.GetSafeNormal();
		PrevAimDirection = AimDirection;
		return AimDirection;
	}

	return PrevAimDirection;
}


void UAimingComponent::Fire()
{
	// Spawn bullet at barrel firing socket
	FVector SpawnLocation = GetOwner()->FindComponentByClass<UStaticMeshComponent>()->GetSocketLocation(FName("Projectile"));
	FRotator SpawnRotation = GetOwner()->FindComponentByClass<UStaticMeshComponent>()->GetSocketRotation(FName("Projectile"));
	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletTarget, SpawnLocation, SpawnRotation);

	Bullet->LaunchProjectile(LaunchSpeed);
}
