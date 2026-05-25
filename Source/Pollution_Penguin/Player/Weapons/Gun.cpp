// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Characters/ShooterCharacter.h"
#include "Player/Controllers/InGamePlayerController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "DummyBullet.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//메쉬 부착 관련 파트 
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	MuzzleFlashComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MuzzleFlashComponent"));
	MuzzleFlashComponent->SetupAttachment(Mesh, TEXT("BarrelEndSocket"));
	MuzzleFlashComponent->SetAutoActivate(false);

	bReplicates = true;
}

void AGun::BeginPlay()
{
	Super::BeginPlay();

	PoolManager = Cast<AObjectPoolManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AObjectPoolManager::StaticClass()));
	if (MuzzleFlash && MuzzleFlashComponent)
	{
		MuzzleFlashComponent->SetAsset(MuzzleFlash);
	}

	if (PoolManager)
	{
		if (HasAuthority() && RealBulletFactory)
		{
			PoolManager->PrePool(RealBulletFactory, 1); 
		}
		if (!IsNetMode(NM_DedicatedServer) && DummyBulletFactory)
		{
			PoolManager->PrePool(DummyBulletFactory, 1);
		}
	}
}


void AGun::SpawnBullet() {
	//총알 위치, 각도 설정 
	APawn* OwnerPawn = Cast<APawn>(GetOwner()); //컨트롤 가져오려면 폰만 가능함. 
	if (OwnerPawn == nullptr || !HasAuthority() || PoolManager == nullptr) return;

	FVector Location = OwnerPawn->GetActorLocation();
	FRotator Rotation = OwnerPawn->GetActorRotation();
	//FTransform BarrelEndSocket = Mesh->GetSocketTransform(TEXT("BarrelEndSocket"));

	//총알 실제 생성, 발사!  
	AActor* PooledActor = PoolManager->GetPoolObject(bulletFactory, Location, Rotation);
	ABullet* bullet = Cast<ABullet>(PooledActor);
	TRACE_BOOKMARK(TEXT("Gun : Real Bullet Spawned"));

	if (bullet == nullptr) return;
	bullet->SetActorLocationAndRotation(Location, Rotation);
	bullet->SetOwner(OwnerPawn);
	bullet->ResetObject();
}

void AGun::SpawnDummyBullet()
{
	if (IsNetMode(NM_DedicatedServer)) return;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr || PoolManager == nullptr || DummyBulletFactory == nullptr) return;

	FVector Location = Mesh->GetSocketLocation(TEXT("BarrelEndSocket"));
	FRotator Rotation = OwnerPawn->GetActorRotation();

	AActor* PooledActor = PoolManager->GetPoolObject(DummyBulletFactory, Location, Rotation);
	ADummyBullet* DummyBullet = Cast<ADummyBullet>(PooledActor);
	const TCHAR* NetRole = HasAuthority() ? TEXT("Server") : TEXT("Client");
	TRACE_BOOKMARK(TEXT("[%s] Gun : Dummy Bullet Spawned"), NetRole);

	if (DummyBullet == nullptr) return;
	DummyBullet->SetActorLocationAndRotation(Location, Rotation);
	DummyBullet->SetOwner(OwnerPawn);
	DummyBullet->ResetObject();
}

void AGun::PlayLocalShootEffect()
{
	if (MuzzleFlashComponent)
	{
		MuzzleFlashComponent->Activate(true);
	}
	TRACE_BOOKMARK(TEXT("Gun : Local Effect Spawned"));
}


void AGun::Multicast_PlayShootEffect_Implementation(APawn* Shooter)
{
	if (IsNetMode(NM_DedicatedServer)) return;

	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner && (MyOwner->IsLocallyControlled() || MyOwner->GetLocalRole() == ROLE_AutonomousProxy)) return;

	if (MuzzleFlashComponent)
	{
		MuzzleFlashComponent->Activate(true);
		TRACE_BOOKMARK(TEXT("Gun : Remote Effect Spawned"));
	}
	SpawnDummyBullet();
}
