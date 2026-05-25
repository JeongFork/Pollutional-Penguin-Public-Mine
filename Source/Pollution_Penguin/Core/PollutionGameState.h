// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PollutionGameState.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTION_PENGUIN_API APollutionGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	TArray <class AShooterCharacter*> AlivePlayers;

	const TArray<class AShooterCharacter*> & GetAlivePlayers() const{
		return AlivePlayers;
	}

	UPROPERTY(BlueprintReadOnly, Category = "Glacier")
	TArray <class AGlacierPlatform* > ActiveGlaciers;

	int32 GetGlacierCount() const { return ActiveGlaciers.Num(); }

	void AddGlacier(class AGlacierPlatform* Glacier) { ActiveGlaciers.AddUnique(Glacier); }

	void RemoveGlacier(class AGlacierPlatform* Glacier) { ActiveGlaciers.Remove(Glacier); }
};
