// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interface/IPoolable.h"
#include "MyBulletObjectPoolSubSystem.generated.h"

class UMyBulletObjectPool;
class APooledActor;

USTRUCT()
struct FBulletPoolUnit
{
	GENERATED_BODY()

	UPROPERTY()
	TArray <TObjectPtr<APooledActor>> ActiveBullets;

	UPROPERTY()
	TArray<TObjectPtr<APooledActor>> InactiveBullets;

	UPROPERTY()
	TSubclassOf<APooledActor> BulletClass;

	UPROPERTY()
	int32 PoolSize = 0;
};

UCLASS()
class MYPROJECT_API UMyBulletObjectPoolSubSystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Tick(float DeltaTime) override;

	APooledActor* AcquireBullet(TSubclassOf<APooledActor> BulletClass, FPoolActorPrama Params);

	FBulletPoolUnit& CreatePoolUnit(TSubclassOf<APooledActor> BulletClass);

	void ReleaseBullet(APooledActor* Bullet);

	void ExpandPool(FBulletPoolUnit& Pool, int32 count);


	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UMyBulletObjectPoolSubSystem, STATGROUP_Tickables);
	}
protected:

	UPROPERTY()
	TMap<TSubclassOf<APooledActor>, FBulletPoolUnit> BulletPoolMap;
};
