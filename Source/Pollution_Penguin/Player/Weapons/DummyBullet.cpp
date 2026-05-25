#include "DummyBullet.h"
#include "Engine/World.h"
#include "TimerManager.h"

ADummyBullet::ADummyBullet()
{
	bReplicates = false;	

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Enemy"));
	CollisionComp->SetCollisionProfileName(TEXT("BulletProfile"));
	CollisionComp->SetSphereRadius(13.f);
	RootComponent = CollisionComp;

	BodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	BodyMeshComp->SetupAttachment(CollisionComp);
	BodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMeshComp->SetRelativeScale3D(FVector(10.0f));
	BodyMeshComp->SetRelativeLocation(FVector(0, 0, -12.5f));

	auto sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/LPG2/LPG2Meshes/Bullets/OBJ_SMGRound_01.OBJ_SMGRound_01'"));
	if (sphereMesh.Succeeded())
	{
		BodyMeshComp->SetStaticMesh(sphereMesh.Object);
	}

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(CollisionComp);
	MovementComp->InitialSpeed = 2000.f;
	MovementComp->MaxSpeed = 2000.f;
	MovementComp->bShouldBounce = true; 
	MovementComp->Bounciness = 0.3f;   
	MovementComp->bAutoActivate = false;
}

void ADummyBullet::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADummyBullet::OnSphereComponentBeginOverlap);
}

void ADummyBullet::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	if (OtherActor->ActorHasTag("Enemy") || OtherActor->ActorHasTag("GlacierPlatform"))
	{
		DeactivateObject(); 
	}
}

bool ADummyBullet::IsActive() const { return bIsActive; }

void ADummyBullet::ResetObject()
{
	bIsActive = true;
	BodyMeshComp->SetVisibility(true);

	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	if (MovementComp)
	{
		MovementComp->Velocity = GetActorForwardVector() * MovementComp->InitialSpeed;
		MovementComp->Activate(true);
	}

	GetWorld()->GetTimerManager().SetTimer(LifeSpanTimer, this, &ADummyBullet::DeactivateObject, 2.0f, false);
}

void ADummyBullet::DeactivateObject()
{
	bIsActive = false;
	BodyMeshComp->SetVisibility(false);

	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (MovementComp)
	{
		MovementComp->Velocity = FVector::ZeroVector;
		MovementComp->Deactivate();
	}
	GetWorld()->GetTimerManager().ClearTimer(LifeSpanTimer);
}