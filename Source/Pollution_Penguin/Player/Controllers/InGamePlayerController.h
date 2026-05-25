// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Map/GlacierPlatformArrangement.h"
#include "Core/Managers/SpawnPoint.h"
 #include "InGamePlayerController.generated.h"
/**
 *
 */


UCLASS()
class POLLUTION_PENGUIN_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void CreateGlacierPlatformsHexagonShpae(int countOfHexagonLength, const FVector& impactCoordinate, const TArray<double>& glacierHealth);


	virtual void BeginPlay() override;
private:
	AGlacierPlatformArrangement* FindGPArrangement() const;
	ASpawnPoint* FindSpawnPoint() const;

	ASpawnPoint* SpawnPoint;
	AGlacierPlatformArrangement* GPArragement;

};
