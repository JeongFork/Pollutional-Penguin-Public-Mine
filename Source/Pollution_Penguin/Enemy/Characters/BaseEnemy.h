#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/Interfaces/PoolInterface.h"
#include "BaseEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDiedSignature, ABaseEnemy*, DeadEnemy);

UCLASS()
class POLLUTION_PENGUIN_API ABaseEnemy : public ACharacter, public IPoolInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
	virtual bool IsActive() const override { return !bIsDead; }
	virtual void ResetObject() override { ResetEnemy(); }
	virtual void DeactivateObject() override { DeactivateEnemy(); }

	void ResetEnemy();
	void DeactivateEnemy();

	UPROPERTY(EditAnywhere, Category = "Stats")
	float Speed = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_IsDead)
	bool bIsDead = false;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDiedSignature OnEnemyDied;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Effects")
	void DamageEffect(FVector Location);

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Stats")
	float Health;

	UFUNCTION()
	void OnRep_IsDead();

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UNiagaraSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UNiagaraSystem* DieFlash;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundBase* ImpactEffect;	//���� ������ �� ���� �Ҹ� 

};
