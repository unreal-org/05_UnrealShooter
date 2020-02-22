// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/KismetSystemLibrary.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UNREALMOTION_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// Constructor
	UMainAnimInstance(const FObjectInitializer& ObjectInitializer);

	// Body Parts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Parts")
	FVector RightFootLocation;   // world space
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Parts")
	FVector LeftFootLocation;    // world space
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joint Targets")
	FVector LeftKneeTargetLocation;   // world space
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joint Targets")
	FVector RightKneeTargetLocation;    // world space
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joint Rotations")
	FRotator RightFootRotation;   // world space
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joint Rotations")
	FRotator LeftFootRotation;    // world space

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Parts")
	FRotator RightHandRotation;   // world space

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Parts")
	FRotator Spine3Rotation;    // world space

	// IK Alpha
	UPROPERTY(BlueprintReadOnly, Category = "IK Alpha")
	float LeftFootIKAlpha = 0;
	UPROPERTY(BlueprintReadOnly, Category = "IK Alpha")
	float RightFootIKAlpha = 0;

	// Transition Events
	UFUNCTION(BlueprintCallable)
	void AnimNotify_IdleEntry();
	UFUNCTION(BlueprintCallable)
	void AnimNotify_WalkingEntry();

	// Transition Variables
	UPROPERTY(BlueprintReadOnly, Category = "Transition Variables")
	bool Walking = false;
	UPROPERTY(BlueprintReadOnly, Category = "Transition Variables")
	bool Ready = false;

	// Ready Pose Index
	UPROPERTY(BlueprintReadOnly, Category = "Transition Variables")
	int32 ReadyIndex = 0;

protected:
	// Native initialization override point
	virtual void NativeInitializeAnimation() override;

	// Tick
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

private:
	// Character Reference
	//class AUnrealCharacter* UnrealCharacter = nullptr;

	// State Machine
	FAnimNode_StateMachine *MainState;

	// Lerp
	void TargetLerp(float DeltaTimeX);
	float LerpTime = 0;
	float LerpDuration = 0.3;

	// Clamp
	FRotator RotatorClamp(FRotator TargetRotator, FRotator RotatorClamp);
	
	// Foot Trace
	FName TraceTag = FName(TEXT("TraceTag"));
	FCollisionQueryParams TraceParameters;
	FVector IKFootTrace(int32 Foot);

};
