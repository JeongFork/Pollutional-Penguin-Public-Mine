// Fill out your copyright notice in the Description page of Project Settings.


#include "Squid.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Map/GlacierPlatform.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


ASquid::ASquid()
{
}

void ASquid::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent = FindComponentByClass<UBoxComponent>();
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASquid::OnOverlapBegin);
	}
}

void ASquid::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bCanHit || !OtherActor || !OtherActor->IsA(AGlacierPlatform::StaticClass()))
	{
		return;
	}

	AGlacierPlatform* Glacier = Cast<AGlacierPlatform>(OtherActor);
	if (Glacier)
	{
		HandleGlacierCollision(Glacier);
	}
}

void ASquid::HandleGlacierCollision(AGlacierPlatform* Glacier)
{
	bCanHit = false;

	FVector BounceDirection = (GetActorLocation() - Glacier->GetActorLocation()).GetSafeNormal();
	BounceDirection.Z = 0;
	LaunchCharacter(BounceDirection * 1000.0f, true, true);

	// ºÎµúÈù ºùÇÏ¸¦ Å¸°ÙÀ¸·Î ¾÷µ¥ÀÌÆ® 
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController && AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetGlacier"), Glacier);
	}

	if (Glacier)
	{
		Glacier->AdjustHealth(-30);
	}

	if (GnawEffect)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GnawEffect, Glacier->GetActorLocation());
	}

	GetWorld()->GetTimerManager().SetTimer(HitCooldownTimer, this, &ASquid::ResetHitFlag, 0.001f, false);
}

void ASquid::ResetHitFlag()
{
	bCanHit = true; 
}