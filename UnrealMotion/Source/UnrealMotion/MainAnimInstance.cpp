// Fill out your copyright notice in the Description page of Project Settings.

#include "MainAnimInstance.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineTypes.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimMontage.h"
#include "UnrealCharacter.h"

///////////////////////////////// Constructors ////////////////////////////////////
UMainAnimInstance::UMainAnimInstance(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{}

/////////////////////////////////// Begin Play ///////////////////////////////////////
void UMainAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    if (!ensure(GetSkelMeshComponent())) { return; }
    if (!ensure(GetSkelMeshComponent()->GetOwner())) { return; }

    // Get State
    MainState = GetStateMachineInstanceFromName(FName(TEXT("MainState")));
    UnrealCharacter = Cast<AUnrealCharacter>(GetSkelMeshComponent()->GetOwner());

    // IK Foot Trace parameters
    TraceParameters = FCollisionQueryParams(TraceTag, false);
    TraceParameters.AddIgnoredComponent(Cast<UPrimitiveComponent>(GetSkelMeshComponent()));
    TraceParameters.AddIgnoredActor(Cast<AActor>(GetSkelMeshComponent()->GetOwner()));

}

///////////////////////////////////// Tick ///////////////////////////////////////
void UMainAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
    if (!ensure(MainState)) { return; }

    switch (MainState->GetCurrentState())
    {
        case 0: // Idle
            //IKHands(DeltaTimeX);
            break;
        case 1: // Walking
            break;
        case 2: // Ready
            // Spine 3 follows Camera rotation here
            TargetLerp(DeltaTimeX);
            break;
    }

    LeftFootLocation = IKFootTrace(0);
    RightFootLocation = IKFootTrace(1);
}

/////////////////////////////// Transition Functions //////////////////////////////
void UMainAnimInstance::AnimNotify_IdleEntry()
{
    LeftFootIKAlpha = 0.99;
    RightFootIKAlpha = 0.99;
}

void UMainAnimInstance::AnimNotify_WalkingEntry()
{
    LeftFootIKAlpha = 0.75;
    RightFootIKAlpha = 0.75;

    // UnrealCharacter = Cast<AUnrealCharacter>(GetSkelMeshComponent()->GetOwner());
}

void UMainAnimInstance::SetLoadMontage(UAnimMontage* LoadMontageTarget)
{
    LoadMontage = LoadMontageTarget;
}

// Spine 3 Lerp
void UMainAnimInstance::TargetLerp(float DeltaTimeX)
{
    if (!ensure(GetSkelMeshComponent()->GetOwner())) { return; }

    FRotator TargetSpine3Rotation;
    if (UnrealCharacter) { TargetSpine3Rotation = UnrealCharacter->GetCameraRotation(); }
    // FRotator TargetHandRotation = UnrealCharacter->GetHandOffsetRotation();
    
    LerpTime = 0;
    if (LerpTime < LerpDuration)
    {
        LerpTime += DeltaTimeX;
        Spine3Rotation = FMath::Lerp(Spine3Rotation, TargetSpine3Rotation, LerpTime);
        //RightHandRotation = FMath::Lerp(RightHandRotation, TargetHandRotation, LerpTime);
    }
}

// Clamp Rotation
FRotator UMainAnimInstance::RotatorClamp(FRotator TargetRotator, FRotator ClampRotator)
{
    TargetRotator.Pitch = FMath::ClampAngle(TargetRotator.Pitch, -ClampRotator.Pitch, ClampRotator.Pitch);
    TargetRotator.Yaw = FMath::ClampAngle(TargetRotator.Yaw, -ClampRotator.Yaw, ClampRotator.Yaw);
    TargetRotator.Roll = FMath:: ClampAngle(TargetRotator.Roll, -ClampRotator.Roll, ClampRotator.Roll);

    return TargetRotator;
}

FVector UMainAnimInstance::IKFootTrace(int32 Foot)
{
    if (!ensure(GetSkelMeshComponent())) { return FVector(0, 0, 0); }
    if (!ensure(GetSkelMeshComponent()->GetOwner())) { return FVector(0, 0, 0); }
    //if (!ensure(CapsuleComponent)) { return FVector(0, 0, 0); }    // NOTE: Is supposedly causing crash?

    // Determine which foot
    FName FootName;
    FVector FootSocketLocation;
    if (Foot == 0) {  // Left Foot
        FootName = FName(TEXT("foot_l"));
        LeftKneeTargetLocation = GetSkelMeshComponent()->GetSocketLocation(FName(TEXT("knee_target_l")));
        FootSocketLocation = GetSkelMeshComponent()->GetSocketLocation(FootName);
    } else if (Foot == 1) {   // Right Foot
        FootName = FName(TEXT("foot_r"));
        RightKneeTargetLocation = GetSkelMeshComponent()->GetSocketLocation(FName(TEXT("knee_target_r")));
        FootSocketLocation = GetSkelMeshComponent()->GetSocketLocation(FootName);
    }

    // Determine Trace Start & End points
    float CapsuleHalfHeight = 88;    // Hard code CapsuleHalfHeight for now - CapsuleComponent causing crash
    FVector StartTrace = FVector(FootSocketLocation.X, FootSocketLocation.Y, CapsuleHalfHeight);
    FVector EndTrace = FVector(FootSocketLocation.X, FootSocketLocation.Y, CapsuleHalfHeight - CapsuleHalfHeight - 15);  // 15 = Extended Trace Distance

    FHitResult HitResult(ForceInit);

    if (!ensure(GetWorld())) { return FVector(0, 0, 0); }
    bool HitConfirm = GetWorld()->LineTraceSingleByChannel(   // Line trace for feet
            HitResult,
            StartTrace,
            EndTrace,
            ECollisionChannel::ECC_Visibility,
            TraceParameters
    );

    if (HitConfirm)
    {
        if (!ensure(HitResult.GetActor())) { return FVector(0, 0, 0); }

        // FootOffset Z & FootOffset at Idle
        if ((GetSkelMeshComponent()->GetOwner()->GetVelocity()).Size() < 0.1) { FootSocketLocation.Z = 13.5; }
        else {
            if (FootSocketLocation.Z - HitResult.Location.Z < 13.5) { FootSocketLocation.Z = 15; }
        }
        
        // Foot Rotations - TODO : Fix
        if (Foot == 0) { 
        LeftFootRotation.Roll = UKismetMathLibrary::DegAtan2(HitResult.Normal.Y, HitResult.Normal.Z);
        LeftFootRotation.Pitch = UKismetMathLibrary::DegAtan2(HitResult.Normal.X, HitResult.Normal.Z);
        } else { 
        RightFootRotation.Roll = UKismetMathLibrary::DegAtan2(HitResult.Normal.Y, HitResult.Normal.Z);
        RightFootRotation.Pitch = UKismetMathLibrary::DegAtan2(HitResult.Normal.X, HitResult.Normal.Z);
        } 

        return FootSocketLocation; 
    }
    
    return FootSocketLocation;  // else - don't offset
}

// Play Load Montage
void UMainAnimInstance::PlayLoadMontage()
{
    if (LoadMontage) {
        if (Montage_IsPlaying(LoadMontage) == false) {
            Montage_Play(
                LoadMontage,
                1.5,
                EMontagePlayReturnType::MontageLength,
                0,
                false
            );
        }
    }
}