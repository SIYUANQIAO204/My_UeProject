// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyBulletObjectPoolSubSystem.generated.h"

class UMyBulletObjectPool;

UCLASS()
class MYPROJECT_API UMyBulletObjectPoolSubSystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

	template<typename T> UMyBulletObjectPool* CreatePool(UWorld* World, int32 InitialSize);

	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UMyBulletObjectPoolSubSystem, STATGROUP_Tickables);
	}
protected:
	UPROPERTY()
	TArray<TObjectPtr<UMyBulletObjectPool>> BulletPools;
};
