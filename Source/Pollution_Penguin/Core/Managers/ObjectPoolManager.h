#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPoolManager.generated.h"

USTRUCT()
struct FObjectPoolArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AActor*> PoolItems;
};


UCLASS()
class POLLUTION_PENGUIN_API AObjectPoolManager : public AActor
{
	GENERATED_BODY()

public:
	AObjectPoolManager();

	UFUNCTION(BlueprintCallable, Category = "Pool")
	AActor* GetPoolObject(UClass* ObjectClass, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void PrePool(UClass* ObjectClass, int32 PoolSize);

private:
	UPROPERTY()
	TMap<UClass*, FObjectPoolArray> Pool;
};