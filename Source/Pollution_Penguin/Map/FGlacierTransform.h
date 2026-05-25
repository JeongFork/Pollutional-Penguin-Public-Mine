// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"FGlacierTransform.generated.h"
/**
 * 
 */
class POLLUTION_PENGUIN_API FGlacierTransform
{
public:
	FGlacierTransform();
	~FGlacierTransform();
};

USTRUCT(Atomic, BlueprintType)
struct FGlacierInfo
{
	GENERATED_USTRUCT_BODY()
public:
	FGlacierInfo();
	FGlacierInfo( FVector rotation, FVector location, AActor* actor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector location;
};