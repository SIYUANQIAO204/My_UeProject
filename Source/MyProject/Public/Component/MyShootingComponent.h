// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExtraData/BulletData.h"
#include "MyShootingComponent.generated.h"

class ABallProjectile;
class USceneComponent;
class UCameraShakeBase;
class APooledBullet;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UMyShootingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyShootingComponent();

	void StartShooting();

	void StopShooting();

	void UpdateAim(float DeltaTime);

	FORCEINLINE void SetAimLocation(const FVector& NewAimLocation) { AimLocation = NewAimLocation; }
	
	FORCEINLINE FVector GetAimLocation() const { return AimLocation; }
	
	bool IsAimingFinished() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABallProjectile> BallProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	TSubclassOf<APooledBullet> PooledBulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletData")
	TObjectPtr<UBulletData> BulletData;

	UFUNCTION(BlueprintCallable)
	void ShootBall();
	
	UPROPERTY(VisibleAnywhere)
	FVector AimLocation;

	UPROPERTY(BlueprintReadOnly)
	FVector AimPoint;

	UPROPERTY(BlueprintReadOnly)
	FVector2D TargetCrosshairPosition;

	UPROPERTY(BlueprintReadOnly)
	FVector2D CurrentCrosshairPosition;

	UPROPERTY(EditAnywhere, Category = "Aiming")
	float AimingSpeed = 10.0f;

	FVector ApplySpreadToDirection(const FVector& Direction,bool bIsAiming) const;

	void PlayShotCameraShake(bool bIsAiming) const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere)
	float FireDelay = 0.5f;

	FTimerHandle ShootTimerHandle;

private:
	UPROPERTY(EditAnywhere, Category = "Recoil")
	float HipFireSpreadAngle = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float AimSpreadAngle = 0.8f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float SpreadIncreasePerShot = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float MaxSpreadAngle = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float SpreadRecoveryRate = 7.0f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float CurrentSpreadAngle = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	TSubclassOf<UCameraShakeBase> ShotCameraShakeClass;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float ShotCameraShakeScale = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float CameraShakeScaleAim = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float VerticalKickPerShot = 0.35f; // 每发上抬角度

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float MaxVerticalKick = 4.0f; // 最大上抬上限

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float HorizontalJitter = 4.0f; // 水平随机抖动

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float VerticalRecoverSpeed = 8.0f; // 松开后恢复速度

	UPROPERTY(VisibleAnywhere, Category = "Recoil")
	float CurrentVerticalKick = 0.0f;

};
