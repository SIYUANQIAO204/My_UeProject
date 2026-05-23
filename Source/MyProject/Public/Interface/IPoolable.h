// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPoolable.generated.h"

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
	void OnAcquireFromPool();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Poolable")
	void OnReleaseToPool();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Poolable")
	void PoolTick(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Poolable")
	bool IsFinished() const;

protected:
	virtual void OnAcquireFromPool_Implementation() {}

	virtual void OnReleaseToPool_Implementation() {}

	virtual void PoolTick_Implementation(float DeltaTime) {}

	virtual bool IsFinished_Implementation() const { return false; }
};
