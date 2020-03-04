// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnrealCharacter.generated.h"

class UCameraComponent;
class UAimingComponent;

UCLASS()
class UNREALMOTION_API AUnrealCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnrealCharacter();

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Set Character target Location & Rotation
	void SetCharacterStartLocation(int32 StartLocationID);
	void SetCharacterGameLocation();
	void SetCharacterRotation(FRotator TargetRotation);
	void SetCharacterState(int32 State);

	// For Delayed State Change
	int32 TargetState = 0;

	/** Getter for Current Condition */
	UFUNCTION(BlueprintPure, Category = "Condition")
	FORCEINLINE float GetisAlive() const { return isAlive; }

	/** Setter for Current Condition */
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetisAlive(bool Alive);

	/** Event for taking damage. Overridden from APawn.*/
	UFUNCTION(BlueprintCallable, Category = "Condition")
	float TakeDamage( float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser ) override;

	// Get Camera Rotation for AnimInstance
	UFUNCTION(BlueprintPure, Category = "Condition")
	FORCEINLINE FRotator GetCameraRotation() { return CameraRotation; }

	// Set Camera Rotation for AnimInstance
	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void SetCameraRotation(FRotator CurrentRotation);

	// Set Camera Rotation for AnimInstance
	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void SetGunRotation(FRotator TargetRotation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Current Player Condition
	UPROPERTY(ReplicatedUsing = OnRep_isAlive)
	bool isAlive;

	// RepNotify for changes made to isAlive
	UFUNCTION()
	void OnRep_isAlive();

	/** Response to isAlive being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnisAliveUpdate();

	/** Server function for spawning projectiles.*/
	UFUNCTION(Server, Reliable)
	void HandleShoot();

	/** Server function for loading projectiles.*/
	UFUNCTION(Server, Reliable)
	void HandleLoad();

	/** Server function for loading projectiles.*/
	UFUNCTION(Server, Reliable)
	void HandleDraw(bool DrawState);

	// Camera Rotation for Spine3Rotation in MainAnimInstance
	UPROPERTY(ReplicatedUsing = OnRep_CameraRotation)
	FRotator CameraRotation;

	// RepNotify for changes made to CameraRotation
	UFUNCTION()
	void OnRep_CameraRotation();

	/** Response to CameraRotation being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnCameraRotationUpdate();

	// Camera Rotation for Spine3Rotation in MainAnimInstance
	UPROPERTY(ReplicatedUsing = OnRep_GunRotation)
	FRotator GunRotation;

	// RepNotify for changes made to GunRotation
	UFUNCTION()
	void OnRep_GunRotation();

	/** Response to CameraRotation being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnGunRotationUpdate();


private:
	// References
	class UMainAnimInstance* MainAnimInstance = nullptr;
	UAimingComponent* AimingComponent = nullptr;

	// Character State
	int32 CharacterState = 0;

	// Locations
	FVector StartLocation_0 = FVector(100, 1350, 90.15);
	FVector StartLocation_1 = FVector(100, 1150, 90.15);
	FVector StartLocation_2 = FVector(100, 950, 90.15);
	FVector StartLocation_3 = FVector(100, 750, 90.15);
	FVector GameStartLocation = FVector(851, 1800, 90);

	// Input Functions
	void EscapeMenu();
	void Shoot();
	void Draw();
	void Load();
	void ResetTargets();

	// Move Character to Location
	FVector TargetLocation;
	void MoveCharacterTo(FVector SetTargetLocation, float DeltaTime);

	// Character Lerp
	FRotator LookAtRotation;
	void Lerp(float DeltaTime);
	float LITime = 0;
	float LIDuration = 0.5;

	// Camera Clamp
	void CameraRotationClamp();
	float CameraPitchClamp = 10;
	float CameraYawClamp = 25;

	// Draw
	bool Drawn = false;
	bool Loaded = false;
	void GunLerp(float DeltaTime);

	// Get Hit Location
	FVector HitLocation;
	FVector AimDirection;
	FRotator HandOffsetRotation;
	bool GetHitLocation(FVector& TargetHitLocation);

	// On Mesh Hit
	// UFUNCTION()
	// void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
