// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GlacierSizeUp.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTION_PENGUIN_API UBTTask_GlacierSizeUp : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	UBTTask_GlacierSizeUp();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
    EBTNodeResult::Type InitSize(float &size, UBehaviorTreeComponent &OwnerComp);
};
