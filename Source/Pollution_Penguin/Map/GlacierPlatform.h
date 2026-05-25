// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include"GlacierPlatformArrangement.h"
#include "GlacierPlatform.generated.h"

UCLASS()
class POLLUTION_PENGUIN_API AGlacierPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlacierPlatform();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere) 
		class UStaticMeshComponent* BoxMesh;

	UPROPERTY(EditAnywhere)
		float MaxHealth = 100;



	UPROPERTY(EditAnywhere)
	FVector OutValueRate;

	UPROPERTY(EditAnywhere)
	FVector TargetValueRate;

	UPROPERTY(EditAnywhere)
	float DampedFrequency;

	UPROPERTY(EditAnywhere)
	float DampingRatio;


	UFUNCTION(BlueprintCallable)
		void  AdjustHealth(float amount);

	UPROPERTY(EditAnywhere)
	float DestrutTime=0.02f;

	UPROPERTY(EditAnywhere)
	float ScaleDecraseRatio=1.0f;
		

	

private:
	UPROPERTY(EditAnywhere,Replicated)
	float nowHealth;
	AGlacierPlatformArrangement* gpArrInstance;
};
