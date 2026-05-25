// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "Components/BoxComponent.h"
#include "Squid.generated.h"

UCLASS()
class POLLUTION_PENGUIN_API ASquid : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ASquid();
	bool IsBouncing() const { return !bCanHit; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void HandleGlacierCollision(class AGlacierPlatform* Glacier);

	FTimerHandle HitCooldownTimer;
	bool bCanHit = true;
	void ResetHitFlag();

public:
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* GnawEffect;

private:
	UPROPERTY()
	UBoxComponent* CollisionComponent;
};
