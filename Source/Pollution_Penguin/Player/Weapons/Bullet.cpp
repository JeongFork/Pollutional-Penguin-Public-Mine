#include "Bullet.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "Engine/World.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/Controllers/InGamePlayerController.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	// 충돌체 인스턴스 등록 
	collisionComp = CreateDefaultSubobject <USphereComponent> (TEXT("Enemy"));
	collisionComp->SetCollisionProfileName(TEXT("BulletProfile"));
	collisionComp->SetSphereRadius(13);	//충돌체 반경 
	RootComponent = collisionComp;

	//스태틱 메쉬 부착
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);	//충돌 ㄴㄴ 
	bodyMeshComp->SetRelativeScale3D(FVector(5.0f));	//스태틱 메쉬 크기 
	bodyMeshComp->SetRelativeLocation(FVector(0, 0, -12.5f));	//충돌 범위로부터 얼마나 위치 조정? 

	bodyMeshComp->SetVisibility(false);

	auto sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/LPG2/LPG2Meshes/Bullets/OBJ_SMGRound_01.OBJ_SMGRound_01'"));
	if (sphereMesh.Succeeded())
	{
		bodyMeshComp->SetStaticMesh(sphereMesh.Object);
	}

	//총알 속도, 탄성 
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);
	movementComp->InitialSpeed = 2000;
	movementComp->MaxSpeed = 2000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.3f;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnSphereComponentBeginOverlap);
}

void ABullet::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bHasHitEnemy) return;
	if (!HasAuthority()) return;

	if (!OtherActor || OtherActor == this) return;
	
	//UE_LOG(LogTemp, Warning, TEXT("총알이 닿은 오브젝트: %s"), *OtherActor->GetName());

	if (OtherActor->ActorHasTag("Enemy") || OtherActor->ActorHasTag("GlacierPlatform")){
		DeactivateObject();	//총알 비활성화 

		bHasHitEnemy = true;

		FVector Location = this->GetActorLocation();
		if (GEngine)
		{
			UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
			if (World)
			{
				AInGamePlayerController* pc = Cast<AInGamePlayerController>(UGameplayStatics::GetPlayerController(World, 0));
				if (pc) {
					pc->CreateGlacierPlatformsHexagonShpae(2, Location, { 40,20});
				}
			}
		}
	}
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasAuthority()) return;

	if (this->GetActorLocation().Z < -10)
	{
		FVector Location = this->GetActorLocation();
		if (GEngine)
		{
			UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
			if (World)
			{
				AInGamePlayerController* pc=Cast<AInGamePlayerController>(UGameplayStatics::GetPlayerController(World, 0));
				if (pc) {
					pc->CreateGlacierPlatformsHexagonShpae(2, Location, {40, 20});
				}
			}
		}
		DeactivateObject();
	}
}

bool ABullet::IsActive() const
{
	return bIsActive;
}

void ABullet::ResetObject()
{
	bIsActive = true;
	bHasHitEnemy = false;

	OnRep_IsActive();

	if (movementComp)
	{
		float Speed = movementComp->InitialSpeed > 0.f ? movementComp->InitialSpeed : 2000.f;

		movementComp->Velocity = GetActorForwardVector() * Speed;
		movementComp->Activate(true);
	}

	GetWorld()->GetTimerManager().SetTimer(LifeSpanTimer, this, &ABullet::DeactivateObject, 2.0f, false);
}

void ABullet::DeactivateObject()
{
	bIsActive = false;

	OnRep_IsActive();

	if (movementComp)
	{
		movementComp->Velocity = FVector::ZeroVector;
		movementComp->Deactivate();
	}

	GetWorld()->GetTimerManager().ClearTimer(LifeSpanTimer);
}


void ABullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABullet, bIsActive);
}

void ABullet::OnRep_IsActive()
{
	if (bIsActive)
	{
		SetActorHiddenInGame(false);
		SetActorTickEnabled(true);
		SetActorEnableCollision(false);
	}
	else
	{
		SetActorHiddenInGame(true);
		SetActorTickEnabled(false);
		SetActorEnableCollision(true);
	}
}