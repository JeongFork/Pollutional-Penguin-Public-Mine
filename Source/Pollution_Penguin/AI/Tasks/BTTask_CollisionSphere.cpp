// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CollisionSphere.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AiController.h"
#include "AI/AIUtility/AIUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Core/PollutionGameState.h"
#include "Engine/World.h"

UBTTask_CollisionSphere::UBTTask_CollisionSphere()
{
	NodeName = "CollisionSphere (Async)";
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_CollisionSphere::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController || !AIController->GetPawn())
	{
		return EBTNodeResult::Failed;
	}

	APawn* AIPawn = AIController->GetPawn();
	FVector SearchLocation = AIPawn->GetActorLocation();	// 오징어 위치 
	SearchRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Size"));

	CachedOwnerComp = &OwnerComp;
	CachedPawn = AIPawn;

	FCollisionObjectQueryParams ObjectTypes;
	ObjectTypes.AddObjectTypesToQuery(ECC_GameTraceChannel7);

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(SearchRadius);

	FCollisionQueryParams ActorsToIgnore;
	ActorsToIgnore.AddIgnoredActor(AIPawn); // 자기 자신 무시

	// 비동기 작업이 끝난 후 실행될 델리게이트 바인딩
	FOverlapDelegate OverlapDelegate = FOverlapDelegate::CreateUObject(this, &UBTTask_CollisionSphere::OnOverlapComplete);

	// 비동기 오버랩 요청
	GetWorld()->AsyncOverlapByObjectType(
		SearchLocation,
		FQuat::Identity,
		ObjectTypes,
		CollisionShape,
		ActorsToIgnore,
		&OverlapDelegate
	);

	return EBTNodeResult::InProgress;
}

void UBTTask_CollisionSphere::OnOverlapComplete(const FTraceHandle& TraceHandle, FOverlapDatum& OverlapDatum)
{
	// 몬스터 죽었으면 ㅈㅈ 
	if (!CachedOwnerComp.IsValid() || !CachedPawn.IsValid()) return;

	FVector SearchLocation = CachedPawn->GetActorLocation();	// 오징어 위치 

	TArray<AActor*> GlacierActors;
	for (const FOverlapResult& Result : OverlapDatum.OutOverlaps)
	{
		AActor* Actor = Result.GetActor();
		if (Actor && Actor->IsA(AGlacierPlatform::StaticClass()))
		{
			GlacierActors.Add(Actor);
		}
	}

	AActor* ClosestGlacier = FindClosestTarget(GlacierActors, SearchLocation);

	UBlackboardComponent* BlackboardComp = CachedOwnerComp->GetBlackboardComponent();

	if (BlackboardComp && ClosestGlacier)
	{
		BlackboardComp->SetValueAsObject(TEXT("TargetGlacier"), ClosestGlacier);
		FinishLatentTask(*CachedOwnerComp.Get(), EBTNodeResult::Succeeded);
		return;
	}

	FinishLatentTask(*CachedOwnerComp.Get(), EBTNodeResult::Failed);
}