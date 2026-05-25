// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Controllers/BaseEnemyController.h"
#include "BehaviorTree/BehaviorTreeComponent.h" 
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Enemy/Characters/BaseEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include <Kismet/GameplayStatics.h>


void ABaseEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
	}
}

void ABaseEnemyController::StopAI()
{
	StopMovement();

	if (GetBrainComponent())
	{
		GetBrainComponent()->StopLogic("Dead");
	}

	if (UBlackboardComponent* BBComp = GetBlackboardComponent())
	{
		BBComp->ClearValue(TEXT("Player"));
	}
}

void ABaseEnemyController::RestartAI()
{
	StopMovement();

	if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(GetBrainComponent()))
	{
		BTComp->RestartLogic();
	}
	else if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
	}
}



