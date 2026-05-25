// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTION_PENGUIN_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
	FString playerName;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
	int killCount = 0;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
	int FreezeCount = 0;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
	int DeadCount = 0;

};
