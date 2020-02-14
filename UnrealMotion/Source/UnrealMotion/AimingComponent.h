// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AimingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALMOTION_API UAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAimingComponent();

	// set turret & barrel reference
	UFUNCTION(BluePrintCallable, Category = "Setup")      
	void Initialize(); // UGun* GunToSet

	void AimAt(FVector HitLocation);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:	
	// Gun to Set
	// UGun* Gun = nullptr;	

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 1500;     // meters per second

	void MoveBarrelTowards(FVector AimDirection);

	FVector AimDirection;
	
};
