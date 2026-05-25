// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include<vector>
#include"FGlacierTransform.h"
#include"Player/State/MyPlayerState.h"
#include "NiagaraFunctionLibrary.h"
#include "GlacierPlatformArrangement.generated.h"

UCLASS()
class POLLUTION_PENGUIN_API AGlacierPlatformArrangement : public AActor
{
	GENERATED_BODY()
public:	
	AGlacierPlatformArrangement();
	UFUNCTION(BlueprintCallable)
	void CreateGlacierPlatformInGrid(const FVector& ImpactCoordinate,float health);

	UFUNCTION(BlueprintCallable)
	void CreateGlacierPlatformsSquareShape(int xLength, int yLength);

	UFUNCTION(BlueprintCallable,Server,Reliable)
	void CreateGlacierPlatformsHexagonShpae(int countOfHexagonLength, const FVector& impactCoordinate, const TArray<double>& glacierHealth);

	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float glacierRotatingCreateDelay = 0.05f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float glacierRadialCreateDelay = 0.1f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	TArray<FVector2D> GetPointsOfTriangleWhichBelong(const FVector2D& impactCoordinate) const;

	UFUNCTION(BlueprintCallable)
	FVector2D GetClosestPoint(const TArray<FVector2D>& points,const FVector2D& impactCoordinate) const;

	UFUNCTION(BlueprintCallable)
	void CreateGlacierPlatformHexagonSymettric(const FVector2D& coordinate,const FVector2D& center, float health);

	UFUNCTION(BlueprintCallable)
	 FVector2D GaussRangeOfMinumum(const FVector2D& impactCoordinate) const;

	UFUNCTION(BlueprintCallable)
	TArray<FVector2D> GetLineCordinatesFromGaussRange(const FVector2D& minimumOfRange) const;
	
	UFUNCTION(BlueprintCallable)
	 FVector2D NormalizeGridVector(const FVector2D& vec) const;

	UFUNCTION(BlueprintCallable)
	FVector2D DeNormalizeGridVector(const FVector2D& normalizedVec)const;

	UFUNCTION(BlueprintCallable)
	 FVector2D GetTriangleCenter(const FVector2D& triangleCoordinate1, const FVector2D& triangleCoordinate2, const FVector2D& triangleCoordinate3) const;

	UFUNCTION(BlueprintCallable)
		bool IsImpactPointAboveLine(const TArray<FVector2D>& lineCoordinates, const FVector2D& impactCoordinate) const;

	UFUNCTION(BlueprintCallable)
		FVector2D GetTriangleAnotherCoordinate(const TArray<FVector2D>& lineCordinates, bool isImpacePointAboveLine) const;

	UFUNCTION(BlueprintCallable)
	 FRotator GetTriangleRotation(bool isImpacePointAboveLine) const;

	UFUNCTION(BlueprintCallable)
	AActor* CreateGlacierPlatform(const FVector& coordinate,const FRotator& rotation,float health);


	UFUNCTION(BlueprintCallable)
	AActor* FindExsitingGlacierAt(const FVector2D& triangleCenter) const;

	UFUNCTION(BlueprintCallable)
	bool AddElementToGlacierPlatformMap(FVector2D center,  AActor* createdGlacier);

	
	UFUNCTION()
	void ApplyDamage(AActor* hitActor, float DamageAmount, const FHitResult& hitResult);

	UFUNCTION()
	void InitializeHitArrays();


	const double UNIT_Y = 1.7320508075688772935274463415059; // sqrt(3)

	


	UPROPERTY()
	AMyPlayerState* myPS;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TMap<FVector2D, AActor*> nowGlacierPlatforms;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> BP_GlacierPlatform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D centerPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		double triangleLength = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		double glacierPlatformZ = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NoMonsterRadius;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		double glacierPlatformDownStartZ = 100;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		float createMaxRadius = 5.0f;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DamageFlash;

	TSet<AActor*> hitActors;

	UPROPERTY(EditAnywhere)
		float attackDamage = 30.0f;
};
