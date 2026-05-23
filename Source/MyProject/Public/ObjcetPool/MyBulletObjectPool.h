// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MyBulletObjectPool.generated.h"

class APooledActor;

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyBulletObjectPool : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(UWorld* World , TSubclassOf<APooledActor> InBulletClass, int32 InitialSize);

	APooledActor* AcquireBullet();

	void ReleaseBullet(APooledActor* Bullet);
	
	void PoolTick(float DeltaTime);

protected:
	void ExpandPool(int32 AdditionalSize);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<APooledActor> BulletClass;

	UPROPERTY()
	TArray<TObjectPtr<APooledActor>> BulletPool;

	UPROPERTY()
	TArray<TObjectPtr<APooledActor>> ActiveBullets;

	UPROPERTY()
	TArray<TObjectPtr<APooledActor>> InactiveBullets;

	UPROPERTY()
	TObjectPtr<UWorld> World;

	UPROPERTY()
	int32 DefaultPoolExpandSize = 20;

};
