// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "Player/Characters/ShooterCharacter.h"
#include "AI/AIUtility/AIUtility.h"
#include "Core/PollutionGameState.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerState.h"
#include "Enemy/Controllers/Crab_Controller.h"

//몬스터가 플레이어 위치 딱 포착하고, 해당 방향으로 돌아서는 코드 

UBTTask_ChasePlayer::UBTTask_ChasePlayer()
{
	NodeName = "ChasePlayer";
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APollutionGameState *GameState = GetWorld()->GetGameState<APollutionGameState>();
	if(!GameState) return EBTNodeResult::Failed;

	const TArray <AShooterCharacter*> & AlivePlayers = GameState->GetAlivePlayers();

	if(AlivePlayers.Num()==0){
		return EBTNodeResult::Failed;
	}

	APawn * ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ControlledPawn == nullptr) return EBTNodeResult::Failed;

	AActor* ClosestPlayer = FindClosestTarget(AlivePlayers, ControlledPawn->GetActorLocation()); //가장 가까운 플레이어 찾기

	if (ClosestPlayer == nullptr) {
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr) return EBTNodeResult::Failed;

	UObject* CurrentTargetObj = BlackboardComp->GetValueAsObject(TEXT("Player"));
	if (CurrentTargetObj != ClosestPlayer) {
		if (ACrab_Controller* CrabController = Cast<ACrab_Controller>(OwnerComp.GetAIOwner())) {
			CrabController->OnTargetPlayerFound(Cast<AShooterCharacter>(ClosestPlayer));
		}
		BlackboardComp->SetValueAsObject(TEXT("Player"), ClosestPlayer);
	}

	//블랙보드 값 플레이어 위치, 부울값 업데이트 
	BlackboardComp->SetValueAsVector(TEXT("PlayerLocation"), ClosestPlayer->GetActorLocation());
	BlackboardComp->SetValueAsBool(TEXT("isMoving"), true);
	BlackboardComp->SetValueAsBool(TEXT("isNotSwimming"), false);
	BlackboardComp->SetValueAsObject(TEXT("Player"), ClosestPlayer);

	RotateToActor(OwnerComp, ClosestPlayer); //플레이어 방향으로 회전
	AAIController* Controller = OwnerComp.GetAIOwner();
	Controller->ClearFocus(EAIFocusPriority::Gameplay); //이 노드 끝나고도 플레이어 바라보는 버그 수정 
	
	return EBTNodeResult::Succeeded;
}
