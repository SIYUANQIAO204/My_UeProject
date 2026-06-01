// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPoolable.generated.h"

class UDataAsset;

USTRUCT(BlueprintType)
struct FPoolActorPrama
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn")
	FVector SpawnLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn")
	FRotator SpawnRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn")
	FVector SpawnDirection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn")
	FVector InitialVelocity;

	UPROPERTY()
	TObjectPtr<UDataAsset> ExtraData;

};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UIPoolable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IIPoolable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Poolable")
	void OnAcquireFromPool(FPoolActorPrama Params);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Poolable")
	void OnReleaseToPool();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Poolable")
	void PoolTick(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Poolable")
	bool IsFinished() const;

protected:
	virtual void OnAcquireFromPool_Implementation(FPoolActorPrama Params) {}

	virtual void OnReleaseToPool_Implementation() {}

	virtual void PoolTick_Implementation(float DeltaTime) {}

	virtual bool IsFinished_Implementation() const { return false; }
};
