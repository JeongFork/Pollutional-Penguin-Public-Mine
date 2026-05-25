// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Characters/BaseEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIcontroller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Core/Managers/SpawnPoint.h"
#include "Enemy/Controllers/BaseEnemyController.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	NetUpdateFrequency = 20.0f;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->MoveUpdatedComponent(FVector(1, 1, 1), GetActorRotation(), true);
	GetCharacterMovement()->MoveUpdatedComponent(FVector(-1, -1, -1), GetActorRotation(), true);
}

void ABaseEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!bIsDead && EndPlayReason == EEndPlayReason::Destroyed)
	{
		bIsDead = true;
		OnEnemyDied.Broadcast(this);
	}

	Super::EndPlay(EndPlayReason);
}


float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("몬스터 남은 체력: %f"), Health);
	FVector EffectLocation = DamageCauser ? DamageCauser->GetActorLocation() : GetActorLocation();
	DamageEffect(EffectLocation);

	/*UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			MuzzleFlash,
			GetActorLocation(),
			GetActorRotation(),
			FVector(5.0f),                                // 스케일
			true                                          // 자동 파괴 여부
		);


	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactEfeect, GetActorLocation());
*/
	if (Health <= 0 && !bIsDead)
	{
		bIsDead = true;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DieFlash,
			GetActorLocation(),
			GetActorRotation(),
			FVector(20.0f),
			true
		);

		DeactivateEnemy();
		OnEnemyDied.Broadcast(this);
	}
	return DamageToApply;
}


void ABaseEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseEnemy, Health);
	DOREPLIFETIME(ABaseEnemy, bIsDead);
}

void ABaseEnemy::ResetEnemy()
{
	Health = MaxHealth;
	bIsDead = false;

	if (HasAuthority())
	{
		SetNetDormancy(DORM_Awake); 
		FlushNetDormancy();         
	}

	OnRep_IsDead();

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
		GetCharacterMovement()->UpdateComponentVelocity();
	}

	if (GetMesh())
	{
		GetMesh()->bPauseAnims = false;
	}

	ABaseEnemyController* AIController = Cast<ABaseEnemyController>(GetController());
	if (AIController)
	{
		AIController->RestartAI();
	}     
}

void ABaseEnemy::DeactivateEnemy()
{
	OnRep_IsDead();

	if (HasAuthority())
	{
		SetNetDormancy(DORM_DormantAll);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->SetMovementMode(MOVE_None);
	}

	TArray<UAudioComponent*> AudioComponents;
	GetComponents<UAudioComponent>(AudioComponents);
	for (UAudioComponent* AudioComp : AudioComponents)
	{
		if (AudioComp && AudioComp->IsPlaying())
		{
			AudioComp->Stop();
		}
	}

	if (GetMesh())
	{
		GetMesh()->bPauseAnims = true;
	}

	ABaseEnemyController* AIController = Cast<ABaseEnemyController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
	}
}


void ABaseEnemy::OnRep_IsDead()
{
	if (bIsDead)
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);

		if (GetMesh()) GetMesh()->bPauseAnims = true;
	}
	else
	{
 		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		SetActorTickEnabled(true);

		if (GetMesh()) GetMesh()->bPauseAnims = false;
	}
}

