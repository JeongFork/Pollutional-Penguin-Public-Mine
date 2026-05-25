#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPoolInterface : public UInterface
{
	GENERATED_BODY()
};

class POLLUTION_PENGUIN_API IPoolInterface
{
	GENERATED_BODY()

public:
	virtual bool IsActive() const = 0;
	virtual void ResetObject() = 0;
	virtual void DeactivateObject() = 0;
};