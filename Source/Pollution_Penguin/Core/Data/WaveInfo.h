// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaveInfo.generated.h"
/**
 * 
 */
class POLLUTION_PENGUIN_API WaveInfo
{
public:
	WaveInfo();
	~WaveInfo();
};

USTRUCT(Atomic, BlueprintType)
struct FWaveInfoStrcut {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int crab;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int squid;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	double waveTime;

	FWaveInfoStrcut() {}
	FWaveInfoStrcut(int crab, int squid)
		:crab(crab), squid(squid) {}
};
