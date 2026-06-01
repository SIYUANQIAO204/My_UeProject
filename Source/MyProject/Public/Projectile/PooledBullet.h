// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "enum/TeamTypes.h"
#include "CoreMinimal.h"
#include "ObjectPool/PooledActor.h"
#include "ExtraData/BulletData.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "PooledBullet.generated.h"


class UBulletData;
class USphereComponent;
class UStaticMeshComponent;


/**
 * 
 */
UCLASS()
class MYPROJECT_API APooledBullet : public APooledActor
{
	GENERATED_BODY()
public:
	APooledBullet();

	virtual void OnAcquireFromPool_Implementation(FPoolActorPrama Params) override;

	virtual void PoolTick_Implementation(float DeltaTime) override;

	UFUNCTION()
	virtual void ActivateBullet(FPoolActorPrama Params);

	UFUNCTION()
	virtual void ReleaseBullet();

	UFUNCTION()
	virtual void OnBulletHit(AActor* OtherActor);

	FORCEINLINE UDataAsset* GetBulletData() const { return BulletData; }
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBulletData> BulletData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	FVector Direction = FVector::ForwardVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	FVector Location = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", Meta = (AllowPrivateAccess = "true"))
	FVector Velocity = FVector::ZeroVector;

	FCollisionQueryParams CollisionParams;

	FCollisionShape CollisionShape;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life", Meta = (AllowPrivateAccess = "true"))
	float CurrentLifeTime = 0.f;


	

};
