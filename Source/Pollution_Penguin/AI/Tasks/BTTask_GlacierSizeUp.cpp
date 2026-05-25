// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GlacierSizeUp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GlacierPlatform.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Core/PollutionGameState.h"



UBTTask_GlacierSizeUp::UBTTask_GlacierSizeUp()
{
    NodeName = "GlacierSizeUp";
}

EBTNodeResult::Type UBTTask_GlacierSizeUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return EBTNodeResult::Failed;
    
    float size = BlackboardComp->GetValueAsFloat("Size");

    APollutionGameState* GameState = Cast<APollutionGameState>(GetWorld()->GetGameState());
    if (!GameState)     return EBTNodeResult::Failed;
    if (GameState->GetGlacierCount() <= 0) {
        return InitSize(size, OwnerComp);
    }

    size += 500;
    BlackboardComp->SetValueAsFloat("Size", size);
    return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_GlacierSizeUp::InitSize(float &size, UBehaviorTreeComponent &OwnerComp)
{
    size = 0;
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat("Size", size);
    return EBTNodeResult::Failed;
}
