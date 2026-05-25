// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerController.h"
#include "kismet/GameplayStatics.h"
AGlacierPlatformArrangement* AInGamePlayerController::FindGPArrangement() const {
	return Cast<AGlacierPlatformArrangement>(UGameplayStatics::GetActorOfClass(GetWorld(), AGlacierPlatformArrangement::StaticClass()));
}

 ASpawnPoint* AInGamePlayerController::FindSpawnPoint() const {
	return Cast<ASpawnPoint>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnPoint::StaticClass()));
}


 void AInGamePlayerController::BeginPlay()
{
	GPArragement = FindGPArrangement();
	SpawnPoint = FindSpawnPoint();
}

void AInGamePlayerController::CreateGlacierPlatformsHexagonShpae_Implementation(int countOfHexagonLength, const FVector& impactCoordinate, const TArray<double>& glacierHealth){
	//UE_LOG(LogTemp, Warning, TEXT("CreateGlacierPlatformsHexagonShpae_Implementation is called!"));
	if (!HasAuthority()) return;
	
	if (GPArragement == nullptr) {
		GPArragement = FindGPArrangement();
		//UE_LOG(LogTemp, Warning, TEXT("GPArragement is NULL!"));
		return;
	}
	if (GPArragement != nullptr) {
		GPArragement->CreateGlacierPlatformsHexagonShpae(countOfHexagonLength, impactCoordinate, glacierHealth);
	}
}

