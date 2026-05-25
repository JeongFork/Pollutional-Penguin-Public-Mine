// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PushMove.h"
#include "AIController.h"
#include "Enemy/Characters/Crab.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/AIUtility/AIUtility.h"

//Z값 안주고 그냥 미는 코드 

UBTTask_PushMove::UBTTask_PushMove()
{
	bNotifyTick = true; //TickTask 사용이 가능해짐! 
	NodeName = "PushMove";
}

uint16 UBTTask_PushMove::GetInstanceMemorySize() const
{
	return sizeof(FPushMoveMemory);
}


EBTNodeResult::Type UBTTask_PushMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr) return EBTNodeResult::Failed;
	ACrab* EnemyPawn = Cast<ACrab>(AIController->GetPawn());
	if (EnemyPawn == nullptr) return EBTNodeResult::Failed;

	FPushMoveMemory* MyMemory = CastInstanceNodeMemory<FPushMoveMemory>(NodeMemory);
	MyMemory->Enemy = EnemyPawn;
	
	FVector Location = EnemyPawn->GetActorLocation();
	FVector End = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("PlayerLocation"));

	FVector Dir = End - Location;
	Dir.Z = 0.0f;
	MyMemory->Direction = Dir.GetSafeNormal();

	//CheckForHit(OwnerComp);
	return EBTNodeResult::InProgress;
}

void UBTTask_PushMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	FPushMoveMemory* MyMemory = CastInstanceNodeMemory<FPushMoveMemory>(NodeMemory);

	if (!MyMemory->Enemy.IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}


	//바닥에 Hit이 없을경우 Task 종료 
	if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("isMoving")))
	{
		// if (WalkSoundComponent)
		// {
		//     WalkSoundComponent->Stop();
		// }

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	//움직여!
	else {	//움직이는 중에만 
		ACrab* EnemyPawn = MyMemory->Enemy.Get();
		MoveActor(EnemyPawn, MyMemory->Direction, EnemyPawn->Speed, true, DeltaSeconds);


		// if (!WalkSoundComponent)
		// {
		//     WalkSoundComponent = UGameplayStatics::SpawnSoundAttached(WalkSound, Enemy->GetRootComponent());
		// }
	}
}

