// Fill out your copyright notice in the Description page of Project Settings.


#include "Crab_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"

void ACrab_Controller::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CurrentTargetPlayer.IsValid())
	{
		CurrentTargetPlayer->OnPlayerDied.RemoveDynamic(this, &ACrab_Controller::OnPlayerDeathHandled);
	}

	Super::EndPlay(EndPlayReason);
}


void ACrab_Controller::OnTargetPlayerFound(AShooterCharacter* TargetActor)
{
	if (TargetActor)
	{
		if (CurrentTargetPlayer == TargetActor)
		{
			return;
		}

		if (CurrentTargetPlayer.IsValid())
		{
			CurrentTargetPlayer->OnPlayerDied.RemoveDynamic(this, &ACrab_Controller::OnPlayerDeathHandled);
		}

		CurrentTargetPlayer = TargetActor;
		TargetActor->OnPlayerDied.AddUniqueDynamic(this, &ACrab_Controller::OnPlayerDeathHandled);
	}
}

void ACrab_Controller::OnPlayerDeathHandled()
{
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		BlackboardComp->ClearValue(TEXT("Player"));
	}

	CurrentTargetPlayer = nullptr;
}