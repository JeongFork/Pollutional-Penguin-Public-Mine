// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Core/PollutionGameState.h"
#include "ShooterCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDiedDelegate);

class AGun;

UCLASS()
class POLLUTION_PENGUIN_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerDiedDelegate OnPlayerDied;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	void Shoot();
	UFUNCTION(Server, Unreliable)
	void Server_Shoot();
	
	UFUNCTION()
	void ShotWithSound();

	UFUNCTION(BlueprintCallable, Category = "State")
	void HandleDeath();

	UFUNCTION(BlueprintCallable, Category = "State")
	void HandleRevive();

	UPROPERTY(BlueprintReadWrite,Replicated);
	bool isShooting = false;

	UPROPERTY(BlueprintReadWrite, Replicated, EditAnywhere)
	bool isDead = false;

	UPROPERTY(BlueprintReadWrite, Replicated)
	FVector2D deadPosition;

	UPROPERTY(EditAnywhere);
	UNiagaraSystem* WalkParticle;

	UPROPERTY(EditAnywhere);
	UNiagaraSystem* WaterParticle;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactEfeect;

	UPROPERTY(EditAnywhere)
	double ShootDelay = 0.3;
	
private:
	UPROPERTY(EditAnywhere)
	USoundBase * WalkSound;
	UAudioComponent* WalkSoundComponent;
	double lastShootTime;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Rotate(float AxisValue);

	/*UFUNCTION()
	void UpdateLookAtCursor();*/

	/*UFUNCTION(Server, Reliable)
	void ClientUpdateLookAtCursor(FRotator NewRotation);*/



	// void LookUp(float AxisValue);

	// UPROPERTY(EditAnywhere)
	// float RotationRate = 10.0f;

	UPROPERTY(EditDefaultsOnly)	//런타임동안 바꿀 수 있게 하고 싶지 않음. 
	TSubclassOf<AGun> GunClass;	//AGun의 서브클래스다.. 

	UPROPERTY(Replicated)
	class AGun*  Gun;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health; 

	APlayerController* PlayerController;

	UPROPERTY()
	APollutionGameState* GameState;

public:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	class UNiagaraComponent* WalkParticleComponent;*/

	UPROPERTY()
	class UNiagaraComponent* WaterParticleComponent;
};
