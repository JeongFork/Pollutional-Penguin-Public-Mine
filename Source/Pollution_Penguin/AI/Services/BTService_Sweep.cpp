// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_Sweep.h"
#include "AIcontroller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Map/GlacierPlatform.h"
#include "DrawDebugHelpers.h"


UBTService_Sweep::UBTService_Sweep()
{
	NodeName = "Is Hit? (Sweep)";
	BoxExtent = FVector(50.0, 50.0, 100.0);
}

void UBTService_Sweep::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController * AIController = OwnerComp.GetAIOwner();
	if(AIController == nullptr) return;

	AActor* Enemy = AIController->GetPawn();
	if (Enemy == nullptr) return;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Enemy);

	FCollisionShape CollisionShape;
	CollisionShape.SetBox(FVector3f(BoxExtent));

	FVector StartBox = Enemy->GetActorLocation() + (Enemy->GetActorForwardVector() * 200.0f);
	FVector EndBox = StartBox + (Enemy->GetActorUpVector() * -200.0f);

	//DrawDebugBox(GetWorld(), StartBox + (EndBox - StartBox) * 0.5f, BoxExtent, FQuat::Identity, FColor::Red, false, 1.0f, 0, 5.0f);
	//DrawDebugBoxes(StartBox, EndBox, BoxExtent);

	FHitResult HitResult;

	//충돌 검사 
	bool bHit = Enemy->GetWorld()->SweepSingleByChannel(
		HitResult,
		StartBox,
		EndBox,
		FQuat::Identity,
		ECC_GameTraceChannel6, 
		CollisionShape,
		QueryParams
	);

	UpdateBlackboard(OwnerComp, bHit);

}

void UBTService_Sweep::UpdateBlackboard(UBehaviorTreeComponent& OwnerComp, bool bHit)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bHit);
}


void UBTService_Sweep::DrawDebugBoxes(const FVector& StartBox, const FVector& EndBox, const FVector& InBoxExtent) const {
	const int32 NumLayers = 5; // 박스 몇겹?
	for (int32 i = 0; i < NumLayers; ++i)
	{
		FVector Offset = FVector(0.0f, 0.0f, i * 2.0f); // 겹마다 층수 조금씩 높임 
		DrawDebugBox(GetWorld(), StartBox + (EndBox - StartBox) * 0.5f + Offset, FVector(BoxExtent), FQuat::Identity, FColor::Green, false, 1.0f, 0, 2.0f);
	}

}