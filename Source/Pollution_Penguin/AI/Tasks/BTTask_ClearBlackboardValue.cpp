// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"



UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue()
{
    NodeName = "Clear Blackboard Value";    //BT_Tree에 해당 이름으로 새 노드를 추가한다! 
}

EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());    //블랙보드에서 해당 키값을 지운다!

    return EBTNodeResult::Succeeded;
}
