// Fill out your copyright notice in the Description page of Project Settings.


#include "GlacierPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Core/PollutionGameState.h"

// Sets default values
AGlacierPlatform::AGlacierPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	RootComponent = BoxMesh;
	nowHealth = 0;
	
	bReplicates = true;
	NetUpdateFrequency = 5.0f;
	NetDormancy = DORM_DormantAll;
}

void AGlacierPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGlacierPlatform, nowHealth);
}
// Called when the game starts or when spawned

void AGlacierPlatform::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(0.0f, 0.0f, GetActorScale().Z));
	gpArrInstance = Cast<AGlacierPlatformArrangement>(UGameplayStatics::GetActorOfClass(GetWorld(), AGlacierPlatformArrangement::StaticClass()));

	APollutionGameState* GameState = Cast<APollutionGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->AddGlacier(this);
	}
}

void AGlacierPlatform::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	APollutionGameState* GameState = Cast<APollutionGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->RemoveGlacier(this);
	}
}


void AGlacierPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//float scaleValue = FMath::LogX(1 + MaxHealth, nowHealth);
	float scaleValue = nowHealth>0?(1.0f-ScaleDecraseRatio) + nowHealth / MaxHealth * ScaleDecraseRatio : 0;
	FVector TargetScale=FVector(scaleValue, scaleValue, GetActorScale().Z);

	FVector toSpring = GetActorScale3D();
	
	FMath::SpringDamper<FVector>(toSpring, OutValueRate, TargetScale, TargetValueRate, DeltaTime, DampedFrequency, DampingRatio);
	SetActorScale3D(toSpring);
}

void AGlacierPlatform::AdjustHealth(float amount) {
	if (HasAuthority())
	{
		FlushNetDormancy();
	}

	nowHealth += amount;
	if (nowHealth < 0) {
		if (gpArrInstance == nullptr) {
			//(LogTemp, Warning, TEXT("gpArrInstance is NULL!"));
			gpArrInstance = Cast<AGlacierPlatformArrangement>(UGameplayStatics::GetActorOfClass(GetWorld(), AGlacierPlatformArrangement::StaticClass()));
		}
		const FVector2D* keyVector=gpArrInstance->nowGlacierPlatforms.FindKey(this);
		if (keyVector == nullptr) {
			//UE_LOG(LogTemp, Warning, TEXT("Cannot Find This GlaicerPlatform in Map."));
			return;
		}
		gpArrInstance->nowGlacierPlatforms.Find(*keyVector);
		//UE_LOG(LogTemp, Warning, TEXT("GlacierPlatform is Finded:%s"), ((gpArrInstance->nowGlacierPlatforms.Find(*keyVector)) == nullptr) ? TEXT("NULL") : TEXT("NOT NULL"));

		gpArrInstance->nowGlacierPlatforms.FindAndRemoveChecked(*keyVector);
		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle,
			FTimerDelegate::CreateLambda([&]() {
				this->Destroy();

				}), DestrutTime, false);
		//this->Destroy();
	}
	if (nowHealth > MaxHealth) {
		nowHealth = MaxHealth;
	}
}


