#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Core/Interfaces/PoolInterface.h"
#include "DummyBullet.generated.h"

UCLASS()
class POLLUTION_PENGUIN_API ADummyBullet : public AActor, public IPoolInterface
{
	GENERATED_BODY()

public:
	ADummyBullet();

	virtual bool IsActive() const override;
	virtual void ResetObject() override;
	virtual void DeactivateObject() override;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* BodyMeshComp;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bIsActive = false;
	FTimerHandle LifeSpanTimer;
};