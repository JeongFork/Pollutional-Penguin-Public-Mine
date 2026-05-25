// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_Sweep.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTION_PENGUIN_API UBTService_Sweep : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UBTService_Sweep();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	void UpdateBlackboard(UBehaviorTreeComponent& OwnerComp, bool bHit);
	void DrawDebugBoxes(const FVector& StartBox, const FVector& EndBox, const FVector& InBoxExtent) const;

	FVector BoxExtent;
};
