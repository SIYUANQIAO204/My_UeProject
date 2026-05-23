// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/IPoolable.h"
#include "PooledActor.generated.h"
class UMyBulletObjectPool;

UCLASS()
class MYPROJECT_API APooledActor : public AActor, public IIPoolable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APooledActor();

	void OnAcquireFromPool_Implementation() override;

	void OnReleaseToPool_Implementation() override;

	bool IsFinished_Implementation() const override;

	void PoolTick_Implementation(float DeltaTime) override;

	void SetPool(UMyBulletObjectPool* InPool);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TObjectPtr<UMyBulletObjectPool> Pool;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool bIsFinished = false;
};
