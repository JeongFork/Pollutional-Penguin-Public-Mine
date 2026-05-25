// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "Enemy/Characters/Crab.h"
#include "Enemy/Characters/Squid.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/TargetPoint.h"
#include "Engine/world.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Enemy/Controllers/Squid_Controller.h"
#include "Misc/OutputDeviceNull.h"
#include "Net/UnrealNetwork.h"
#include "Player/Controllers/InGamePlayerController.h"
#include "Core/Managers/ObjectPoolManager.h"

ASpawnPoint::ASpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	currentTime = 0.0f;
}

void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	PoolManager = GetWorld()->SpawnActor<AObjectPoolManager>
		(AObjectPoolManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	TargetPoints.Empty();
	//태그명이 Spawn인 것들 찾아서 TargetPoints 배열에 추가함
	TArray<AActor*> FoundPoints;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Spawn"), FoundPoints);

	for (AActor* Actor : FoundPoints)
	{
		ATargetPoint* TargetPoint = Cast<ATargetPoint>(Actor);
		if (TargetPoint)
		{
			TargetPoints.Add(TargetPoint);
		}
	}
	lastSpawnTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	lastRoundTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
	if (World)
	{
		AInGamePlayerController* pc = Cast<AInGamePlayerController>(UGameplayStatics::GetPlayerController(World, 0));
		if (pc) {
			pc->GoToNextWave();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("PC is Missing: SpawnPoint"));
		}
	}*/
}
void ASpawnPoint::SpawnEnemy(UClass* spawnMonster, FVector spawnLocation, FRotator spawnRotation)
{
	if (TargetPoints.Num() <= 0) {
		return;
	}
	if (!HasAuthority()) return;

	if (PoolManager)
	{
		ABaseEnemy* SpawnedEnemy = Cast<ABaseEnemy>(PoolManager->GetPoolObject(spawnMonster, spawnLocation, spawnRotation));
		if (SpawnedEnemy)
		{
			ActiveEnemyCount++;      

			if (!SpawnedEnemy->OnEnemyDied.IsAlreadyBound(this, &ASpawnPoint::HandleEnemyDied))
			{
				SpawnedEnemy->OnEnemyDied.AddDynamic(this, &ASpawnPoint::HandleEnemyDied);
			}
		}
	}
}

void ASpawnPoint::HandleEnemyDied(ABaseEnemy* DeadEnemy)
{
	ActiveEnemyCount--; 

	if (ActiveEnemyCount <= 0)
	{
		ActiveEnemyCount = 0;
	}
}

bool ASpawnPoint::GoToNextWave()
{
	if (NowSpawnTerm() < nextSpawnTerm) {
		return false;
	}

	areAllEnemyDead = (ActiveEnemyCount <= 0);

	//만약 마지막 라운드,웨이브면 Wave,Round 증가하지 않음 return false;-> 모두 죽으면 게임 End
	if (nowRound + 1 >= RoundInfos.Num()) {
		FRoundInfoStruct roundInfo = RoundInfos[nowRound];
		if (nowWave + 1 >= roundInfo.waveInfos.Num()) {
			if (areAllEnemyDead) {
				GameEnd_Blueprint();
			}
			return false;
		}
	}

	//만약 해당 라운드의 마지막 wave면 Round++; wave=0; 그리고 만약 이와 동시에 몬스터 모두 죽어있으면 return true; 아니면 return false;
	FRoundInfoStruct roundInfo = RoundInfos[nowRound];
	FWaveInfoStrcut waveInfo = roundInfo.waveInfos[nowWave];

	if (nowWave + 1 >= roundInfo.waveInfos.Num()) {
		if (areAllEnemyDead) {
			SetRoundAndWave(nowRound + 1, 0);
			lastRoundTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
			lastSpawnTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
			nextSpawnTerm = waveInfo.waveTime;
			//UE_LOG(LogTemp, Warning, TEXT("This is Last Wave of this Round."));

			return true;
		}
		else {
			return false;
		}
	}

	//-> 마지막 라운드도 아니고 마지막 wave도 아니다(일반적인 라운드니 시간 체크하고 Wave 별로 넘어가야한다.) return true;
	SetRoundAndWave(nowRound, nowWave + 1);
	lastSpawnTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	nextSpawnTerm = waveInfo.waveTime;
	//UE_LOG(LogTemp, Warning, TEXT("Go To Next Wave"));
	return true;
}

double ASpawnPoint::NowSpawnTerm()
{
	return  UGameplayStatics::GetRealTimeSeconds(GetWorld()) - lastSpawnTime;
}

void ASpawnPoint::SpawnWave_Direct(int round, int wave)
{
	if (round < 0 || round >= RoundInfos.Num()) {
		//UE_LOG(LogTemp, Error, TEXT("Round index is less than 0 or grater and equal than Total round count."));
		return;
	}
	FRoundInfoStruct nowRoundInfo = RoundInfos[round];

	if (wave < 0 || wave >= nowRoundInfo.waveInfos.Num()) {

		//UE_LOG(LogTemp, Error, TEXT("wave index is less than 0 or grater and equal than this round's wave count."));
		return;
	}
	FWaveInfoStrcut waveInfo = nowRoundInfo.waveInfos[wave];
	for (int i = 0; i < waveInfo.crab; i++) {
		FTimerHandle UniqueHandle;
		FTimerDelegate timeDel;
		int32 rand = FMath::RandRange(0, TargetPoints.Num() - 1);
		FVector spawnLocation = TargetPoints[rand]->GetActorLocation();
		FRotator spawnRotation = TargetPoints[rand]->GetActorRotation();
		timeDel.BindUFunction(this, FName("SpawnEnemy"), myEnemy[0], spawnLocation, spawnRotation);
		if (GEngine)
		{
			UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
			if (World)
			{
				World->GetTimerManager().SetTimer(UniqueHandle, timeDel, (i * 2 + 1) * MonsterSpawnDelay, false);
			}
		}
	}

	for (int i = 0; i < waveInfo.squid; i++) {
		FTimerHandle UniqueHandle;
		FTimerDelegate timeDel;
		int32 rand = FMath::RandRange(0, TargetPoints.Num() - 1);
		FVector spawnLocation = TargetPoints[rand]->GetActorLocation();
		FRotator spawnRotation = TargetPoints[rand]->GetActorRotation();
		timeDel.BindUFunction(this, FName("SpawnEnemy"), myEnemy[1], spawnLocation, spawnRotation);
		if (GEngine)
		{
			UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
			if (World)
			{
				World->GetTimerManager().SetTimer(UniqueHandle, timeDel, (i * 2) * MonsterSpawnDelay + 0.01f, false);
			}
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("NowRound:%d"), nowRound);
	//UE_LOG(LogTemp, Warning, TEXT("NowWave:%d"), nowWave);
}

void ASpawnPoint::SetRoundAndWave(int round, int wave)
{
	nowRound = round;
	nowWave = wave;
}

void ASpawnPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASpawnPoint, nowRound);
	DOREPLIFETIME(ASpawnPoint, nowWave);
	DOREPLIFETIME(ASpawnPoint, areAllEnemyDead);
}