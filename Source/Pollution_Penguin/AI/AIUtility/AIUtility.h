#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

template<typename T>
T * FindClosestTarget(const TArray<T*>& Actors, const FVector& Location){
    FVector Start = Location; 
    float ClosestDistance = MAX_FLT;
    T* ClosestPlayer = nullptr;

    for (T* Actor : Actors)
    {
        if (Actor)
        {
            float Distance = FVector::Dist(Start, Actor->GetActorLocation()); 
            if (Distance < 0.1f)
                continue; 
            if (Distance < ClosestDistance)
            {
                ClosestDistance = Distance;
                ClosestPlayer = Actor; // 가장 가까운 액터 저장
            }
        }
    }
    return ClosestPlayer;
}


void RotateToActor(UBehaviorTreeComponent& OwnerComp, AActor* Target, float DeltaSeconds = 0.0f);

void MoveActor(AActor* Actor, const FVector& Direction, float Speed, bool bKeepZ, float DeltaSeconds);
void MoveActorWithInertia(AActor* Actor, const FVector& Direction, float & Speed, bool bKeepZ, float DeltaSeconds);
