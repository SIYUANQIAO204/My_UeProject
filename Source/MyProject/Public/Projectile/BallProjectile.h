// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/*#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class MYPROJECT_API ABallProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallProjectile();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <USphereComponent> SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovementComponent; }

	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class MYPROJECT_API ABallProjectile : public AActor
{
	GENERATED_BODY()

public:
	ABallProjectile();

private:

	UPROPERTY(VisibleAnywhere, Category = "Ball")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Ball")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

protected:

	virtual void BeginPlay() override;

	/** 子弹检测玩家 */
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:

	virtual void Tick(float DeltaTime) override;
};