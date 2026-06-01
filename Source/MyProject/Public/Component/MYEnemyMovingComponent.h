// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MYEnemyMovingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UMYEnemyMovingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMYEnemyMovingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetTargetLocation(const FVector& NewTargetLocation);

	UFUNCTION()
	void StopMove();

	UFUNCTION()
	void TickMove(float DeltaTime);

private:
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector Location;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float MaxSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Acceleration")
	float MaxAcceleration = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float ArriveRadius = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SlowRadius = 200.0f;
};
