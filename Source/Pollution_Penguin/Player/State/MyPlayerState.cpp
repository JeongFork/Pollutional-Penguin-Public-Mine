// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "Net/UnrealNetwork.h"
// Sets default values

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyPlayerState, killCount);
	DOREPLIFETIME(AMyPlayerState, playerName);
	DOREPLIFETIME(AMyPlayerState, FreezeCount);
	DOREPLIFETIME(AMyPlayerState, DeadCount);
	
}