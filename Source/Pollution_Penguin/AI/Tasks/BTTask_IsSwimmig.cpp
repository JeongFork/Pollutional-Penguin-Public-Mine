// // Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IsSwimmig.h"
#include "AIcontroller.h"
#include "Enemy/Characters/Crab.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/AIUtility/AIUtility.h"
#include "Kismet/GameplayStatics.h"



UBTTask_IsSwimmig::UBTTask_IsSwimmig()
{
	bNotifyTick = true; //TickTask 사용이 가능해짐! 
	NodeName = "IsSwimming";
}

// 메모리 크기 할당
uint16 UBTTask_IsSwimmig::GetInstanceMemorySize() const 
{
	return sizeof(FIsSwimmingMemory);
}

EBTNodeResult::Type UBTTask_IsSwimmig::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr) return EBTNodeResult::Failed;

	ACrab* EnemyPawn = Cast<ACrab>(AIController->GetPawn());
	if (!EnemyPawn) return EBTNodeResult::Failed;

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("isNotSwimming")) == true) {
		return EBTNodeResult::Succeeded;
	}

	// 캐싱하기 
	FIsSwimmingMemory* MyMemory = reinterpret_cast<FIsSwimmingMemory*>(NodeMemory);
	MyMemory->Enemy = EnemyPawn;

	FVector Location = EnemyPawn->GetActorLocation();
	FVector End = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("PlayerLocation"));
	MyMemory->Direction = (End - Location).GetSafeNormal();

	return EBTNodeResult::InProgress;

}



void UBTTask_IsSwimmig::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FIsSwimmingMemory* MyMemory = reinterpret_cast<FIsSwimmingMemory*>(NodeMemory);

	if(!MyMemory->Enemy.IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ACrab* EnemyPawn = MyMemory->Enemy.Get();

	// 움직여!
	if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("isNotSwimming"))) 
	{
		MoveActor(EnemyPawn, MyMemory->Direction, EnemyPawn->Speed, false, DeltaSeconds);
		//if (!SwimSoundComponent)
		// 	 {
		// 	     SwimSoundComponent = UGameplayStatics::SpawnSoundAttached(SwimSound, Enemy->GetRootComponent());
		// 	 }
	}
	else
	{
		//if (SwimSoundComponent)
		// 	 {
		// 	     SwimSoundComponent->Stop();
		// 	 }
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	
}

