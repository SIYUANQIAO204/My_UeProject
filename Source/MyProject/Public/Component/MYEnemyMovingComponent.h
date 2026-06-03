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

	UFUNCTION()
	void TickRotate(float DeltaTime);

	UFUNCTION()
	void SwitchPathPoint();

	UFUNCTION()
	FORCEINLINE bool IsMoving() const { return bIsMoving; }

private:
	UPROPERTY(VisibleAnywhere, Category = "Owner", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> Owner;

	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving = false;

	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess="true"))
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	int PathIndex;

	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess="true"))	
	TArray<FVector> PathPoints;

	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess="true"))
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (AllowPrivateAccess="true"))
	FVector Location;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess="true"))
	float MaxSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Acceleration", meta = (AllowPrivateAccess="true"))
	float MaxAcceleration = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess="true"))
	float ArriveRadius = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess="true"))
	float SlowRadius = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation", meta = (AllowPrivateAccess="true"))
	float MaxRotationSpeed = 180.0f;
};
