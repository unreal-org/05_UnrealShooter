// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "UnrealMotionGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/UnrealString.h"
#include "GameFramework/PlayerStart.h"

// AActor* AUnrealMotionGameModeBase::ChoosePlayerStart(AController* Player)
// {
//     if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true).Equals(FString("ShowdownMap"), ESearchCase::IgnoreCase)) {
//         if (GetNumPlayers() == 1) {
//             for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It) {
//                 APlayerStart* Target = *It;
// 		        if (Target && Target->GetName() == FString("PlayerStart_4")) {
// 			        return Target;
// 		        }
//             }
//         }
//         else {
//             for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It) {
//                 APlayerStart* Target = *It;
// 		        if (Target && Target->GetName() == FString("PlayerStart2_7")) {
// 			        return Target;
// 		        }
//             }
//         }
//     }
//     else
//     {
//         for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It) {
//             APlayerStart* Target = *It;
// 		    if (Target) {
// 			    return Target;
// 		    }
//         }
//     }
    
// }