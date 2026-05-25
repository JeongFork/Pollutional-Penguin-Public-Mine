// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Core/Data/WaveInfo.h"
#include "RoundInfo.generated.h"
/**
 * 
 */
class POLLUTION_PENGUIN_API RoundInfo
{
public:
	RoundInfo();
	~RoundInfo();
};

USTRUCT(Atomic,BlueprintType)
struct FRoundInfoStruct {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FWaveInfoStrcut> waveInfos;

};