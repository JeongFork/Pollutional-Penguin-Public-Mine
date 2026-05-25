#include "ObjectPoolManager.h"
#include "Core/Interfaces/PoolInterface.h" 
#include "Engine/World.h"

AObjectPoolManager::AObjectPoolManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

AActor* AObjectPoolManager::GetPoolObject(UClass* ObjectClass, FVector Location, FRotator Rotation)
{
	if (!ObjectClass) return nullptr;

	FObjectPoolArray& PoolArray = Pool.FindOrAdd(ObjectClass);
	AActor* SelectedObject = nullptr;

	for (AActor * PoolItem : PoolArray.PoolItems)
	{
		if (PoolItem)
		{
			IPoolInterface* PoolableItem = Cast<IPoolInterface>(PoolItem);
			if (PoolableItem && !PoolableItem->IsActive())
			{
				SelectedObject = PoolItem;
				break;
			}
		}
	}

	// ��밡���Ѱ� ���ٸ� �߰� 
	if (!SelectedObject)
	{
		SelectedObject = GetWorld()->SpawnActor<AActor>(ObjectClass, Location, Rotation);
		if (SelectedObject)
		{
			PoolArray.PoolItems.Add(SelectedObject);

			IPoolInterface* PoolableItem = Cast<IPoolInterface>(SelectedObject);
			if (PoolableItem)
			{
				PoolableItem->ResetObject(); 
			}
		}
	}
	// ��밡���Ѱ� �ִٸ� ����
	else
	{
		SelectedObject->SetActorLocationAndRotation(Location, Rotation);

		IPoolInterface* PoolableItem = Cast<IPoolInterface>(SelectedObject);
		if (PoolableItem)
		{
			PoolableItem->ResetObject(); 
		}
	}

	return SelectedObject;
}

void AObjectPoolManager::PrePool(UClass* ObjectClass, int32 PoolSize)
{
	if (!ObjectClass || !GetWorld()) return;

	FObjectPoolArray& PoolArray = Pool.FindOrAdd(ObjectClass);

	for (int32 i = 0; i < PoolSize; ++i)
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ObjectClass, FVector::ZeroVector, FRotator::ZeroRotator);

		if (SpawnedActor)
		{
			PoolArray.PoolItems.Add(SpawnedActor);

			IPoolInterface* PoolableItem = Cast<IPoolInterface>(SpawnedActor);
			if (PoolableItem)
			{
				PoolableItem->DeactivateObject();
			}
		}
	}
}