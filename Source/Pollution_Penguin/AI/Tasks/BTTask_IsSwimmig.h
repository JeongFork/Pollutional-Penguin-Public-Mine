// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h" 
#include "Enemy/Characters/Crab.h"
#include "Components/AudioComponent.h" 
#include "BTTask_IsSwimmig.generated.h"


struct FIsSwimmingMemory
{
	TWeakObjectPtr<ACrab> Enemy;
	FVector Direction;
	//TWeakObjectPtr<UAudioComponent> SwimSoundComponent;
};


UCLASS()
class POLLUTION_PENGUIN_API UBTTask_IsSwimmig : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UBTTask_IsSwimmig();
	virtual uint16 GetInstanceMemorySize() const override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	USoundBase* SwimSound;
};
