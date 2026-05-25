// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SquidMove.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "AI/AIUtility/AIUtility.h"
#include "AIController.h"

UBTTask_SquidMove::UBTTask_SquidMove()
{
	NodeName = "SquidMove";
	bNotifyTick = true;
}

uint16 UBTTask_SquidMove::GetInstanceMemorySize() const
{
	return sizeof(FSquidMoveMemory);
}

EBTNodeResult::Type UBTTask_SquidMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ASquid* EnemyPawn = Cast<ASquid>(AIController->GetPawn());
	if (!EnemyPawn) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetGlacier")));
	if (!Target) return EBTNodeResult::Failed;

	FSquidMoveMemory* MyMemory = CastInstanceNodeMemory<FSquidMoveMemory>(NodeMemory);
	MyMemory->EnemySquid = EnemyPawn;
	MyMemory->TargetGlacier = Target;

	return EBTNodeResult::InProgress;
}



void UBTTask_SquidMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FSquidMoveMemory* MyMemory = CastInstanceNodeMemory<FSquidMoveMemory>(NodeMemory);

	if (!MyMemory->EnemySquid.IsValid() || !MyMemory->TargetGlacier.IsValid()) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ASquid* Squid = MyMemory->EnemySquid.Get();
	AActor* Target = MyMemory->TargetGlacier.Get();

	if (Squid->IsBouncing()) {
		return;
	}

	//해당 방향으로 이동 
	MoveTarget(OwnerComp, Squid, Target, DeltaSeconds);
}


void UBTTask_SquidMove::MoveTarget(UBehaviorTreeComponent& OwnerComp, ASquid* Squid, AActor* TargetGlacier, float DeltaSeconds)
{
	if (Squid && TargetGlacier)
	{
		FVector Direction = TargetGlacier->GetActorLocation() - Squid->GetActorLocation();
		Direction.Z = 0.0f; 
		Direction.Normalize();

		Squid->AddMovementInput(Direction, 1.0f);
		//MoveActor(Squid, Direction, Squid->Speed, false, DeltaSeconds);


		FRotator TargetRotation = Direction.Rotation();
		RotateToActor(OwnerComp, TargetGlacier, DeltaSeconds);		
	}
}
