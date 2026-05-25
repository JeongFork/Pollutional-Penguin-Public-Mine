// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "Enemy/Characters/Squid.h"
#include "Map/GlacierPlatform.h"
#include "Components/BoxComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


#include "BTTask_SquidMove.generated.h"

struct FSquidMoveMemory
{
	TWeakObjectPtr<ASquid> EnemySquid;
	TWeakObjectPtr<AActor> TargetGlacier;
};

UCLASS()
class POLLUTION_PENGUIN_API UBTTask_SquidMove : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SquidMove();
	virtual uint16 GetInstanceMemorySize() const override;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void MoveTarget(UBehaviorTreeComponent& OwnerComp, ASquid* Squid, AActor* TargetGlacier, float DeltaSeconds);
};
