// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyController.h"
#include "Player/Characters/ShooterCharacter.h"
#include "Crab_Controller.generated.h"

/**
 *
 */
UCLASS()
class POLLUTION_PENGUIN_API ACrab_Controller : public ABaseEnemyController
{
	GENERATED_BODY()

public:
	void OnTargetPlayerFound(AShooterCharacter* TargetActor);

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnPlayerDeathHandled();

	TWeakObjectPtr<class AShooterCharacter> CurrentTargetPlayer;
};
