// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyController.generated.h"

/**
 *
 */
UCLASS()
class POLLUTION_PENGUIN_API ABaseEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	void RestartAI();
	void StopAI();


protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* AIBehavior;
};
