// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Enemy/Characters/Squid.h"
#include "GlacierPlatform.h"
#include "CollisionQueryParams.h" 
#include "Engine/OverlapResult.h"

#include "BTTask_CollisionSphere.generated.h"

UCLASS()
class POLLUTION_PENGUIN_API UBTTask_CollisionSphere : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CollisionSphere();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 비동기 쿼리가 완료되었을 때 호출될 콜백 함수
	void OnOverlapComplete(const FTraceHandle& TraceHandle, FOverlapDatum& OverlapDatum);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search")
	float SearchRadius;

private:
	// 비동기 작업 완료 후 FinishLatentTask를 호출하기 위해 컴포넌트 캐싱
	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
	TWeakObjectPtr<APawn> CachedPawn;
};