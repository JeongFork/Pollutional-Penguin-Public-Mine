// Fill out your copyright notice in the Description page of Project Settings.


#include "FGlacierTransform.h"

FGlacierTransform::FGlacierTransform()
{

}

FGlacierTransform::~FGlacierTransform()
{
}

FGlacierInfo::FGlacierInfo()
	:actor(nullptr),rotation(FVector()),location(FVector())
{
}

FGlacierInfo::FGlacierInfo(FVector rotation, FVector location, AActor* actor=nullptr)
	:actor(actor),rotation(rotation),location(location)
{
}
