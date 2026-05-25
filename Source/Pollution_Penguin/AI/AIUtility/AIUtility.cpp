#include "AIUtility.h"
#include "AIController.h"
#include "Player/Characters/ShooterCharacter.h"



//Target 방향으로 회전
void RotateToActor(UBehaviorTreeComponent& OwnerComp, AActor* Target, float DeltaSeconds)
{
    if(Target == nullptr)
        return;

    AAIController* Controller = OwnerComp.GetAIOwner();
    if (Controller) {
        APawn* ControlledPawn = Controller->GetPawn();

        if(ControlledPawn == nullptr)
            return;

        FVector DirectionToPlayer = Target->GetActorLocation() - ControlledPawn->GetActorLocation();
        DirectionToPlayer.Z = 0; // Z축 회전 ㄴㄴ
        
        FRotator TargetRotation = DirectionToPlayer.Rotation(); // 목표 회전값
        if (DeltaSeconds == 0.0f){
            ControlledPawn->SetActorRotation(TargetRotation);
        }

        else if (DeltaSeconds != 0.0f){
            FRotator CurrentRotation = ControlledPawn->GetActorRotation();

            FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, 5.0f); // 회전 보간
            NewRotation.Pitch = 0.0f; 
            NewRotation.Roll = 0.0f;
            ControlledPawn->SetActorRotation(NewRotation);
        }
        
        //Controller->SetFocus(Target);
    }
}

//bKeepZ가 true면 움직이던중 중력 영향 받게끔 설정함. 
void MoveActor(AActor* Actor, const FVector& Direction, float Speed, bool bKeepZ, float DeltaSeconds)
{
    FVector Location = Actor->GetActorLocation();
    Location.X += Speed * Direction.X * DeltaSeconds;
    Location.Y += Speed * Direction.Y * DeltaSeconds;
    if (bKeepZ)
        Location.Z = Actor->GetActorLocation().Z; // Z축은 현재 위치 유지
    Actor->SetActorLocation(Location);
}

void MoveActorWithInertia(AActor* Actor, const FVector& Direction, float & Speed, bool bKeepZ, float DeltaSeconds)
{
    FVector Location = Actor->GetActorLocation();
    
    const float DecelerationFactor = 0.9f; //감속 계수
    Speed *= DecelerationFactor; // 속도 감소

    Location.X += Speed * Direction.X * 2 * DeltaSeconds;
    Location.Y += Speed * Direction.Y * 2 * DeltaSeconds;
    
    if (bKeepZ)
        Location.Z = Actor->GetActorLocation().Z; 

    Actor->SetActorLocation(Location);
}