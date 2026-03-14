// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class USightComponent;
class ABallProjectile;
class UMyShootingComponent;
class UMyEnemyPatrolComponent;

UCLASS()
class MYPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	//bool LineTraceActor(AActor* TargetActor);

	//bool CanSeeActor(const AActor* TargetActor, FVector Start, FVector End) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<ABallProjectile> BallProjectileClass;

	//void ShootBall();

	bool bCanSeePlayer = false;
	bool bPreviousCanSeePlayer = false;

	FTimerHandle ShootTimerHandle;

	UPROPERTY(EditAnywhere)
	float ShootInterval = 3.0f;

	float FireDelay = 0.5f;

	void InitTargertCharacter();

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMyShootingComponent> ShootingComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USightComponent> SightComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMyEnemyPatrolComponent> PatrolComponent;

private:
	TObjectPtr<ACharacter> TargetCharacter;


};
