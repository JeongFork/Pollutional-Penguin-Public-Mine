// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h" 
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h" 
#include "BTTask_PushMove.generated.h"


class ACrab;

struct FPushMoveMemory
{
	TWeakObjectPtr<ACrab> Enemy;
	FVector Direction;
	//TWeakObjectPtr<UAudioComponent> WalkSoundComponent;
};

UCLASS()
class POLLUTION_PENGUIN_API UBTTask_PushMove : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PushMove();
	virtual uint16 GetInstanceMemorySize() const override;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	USoundBase* WalkSound; 
};
