// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MyShootingComponent.h"
#include "Projectile/BallProjectile.h"
#include "Interface/TeamInterface.h"
#include "MyPlayer.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectPool/MyBulletObjectPoolSubSystem.h"
#include "Projectile/PooledBullet.h"
#include "Camera/CameraShakeBase.h"
#include "Component/RecoilComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMyShootingComponent::UMyShootingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


float UMyShootingComponent::GetImpluseAxisValue(FAxisNoiseConfig AxisConfig) const
{
	while (true)
	{
		float U1 = FMath::FRand();
		float U2 = FMath::FRand();
		float Value = FMath::Sqrt(-2.0f * FMath::Loge(U1)) * FMath::Cos(2.0f * PI * U2);
		Value *= AxisConfig.Sigma;
		if(abs(Value) <= AxisConfig.MaxAbs)
		{
			return Value + AxisConfig.Mean;
		}
	}
	return 0.0f;
}

bool UMyShootingComponent::IsAimingFinished() const
{
	FVector ForwardVector = GetOwner()->GetActorForwardVector();
	FVector ToAimTarget = (AimLocation - GetOwner()->GetActorLocation()).GetSafeNormal();
	float DotProduct = FVector::DotProduct(ForwardVector, ToAimTarget);
	return DotProduct > 0.99f;
}

// Called when the game starts
void UMyShootingComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!BulletData)
	{
		UE_LOG(LogTemp, Warning, TEXT("BulletData is null in MyShootingComponent::BeginPlay"));
		return;
	}
	BulletData->BulletOwner = GetOwner();
	if (GetOwner() && GetOwner()->Implements<UTeamInterface>())
	{
		BulletData->Team = ITeamInterface::Execute_GetTeam(GetOwner());
	}
	// ...

}


void UMyShootingComponent::ShootBall()
{
	UE_LOG(LogTemp, Log, TEXT("MyShootingComponent::ShootBall called on %s"), *GetOwner()->GetName());
	if (!BulletData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShootBall aborted: BulletData is null"));
		return;
	}
	FPoolActorPrama Params;
	Params.ExtraData = BulletData;
	UMyBulletObjectPoolSubSystem* Pool = GetWorld()->GetSubsystem<UMyBulletObjectPoolSubSystem>();
	if (Pool == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bullet pool subsystem not found in MyShootingComponent!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("MyShootingComponent::ShootBall: Pool found. BallProjectileClass=%s PooledBulletClass=%s"), BallProjectileClass ? *BallProjectileClass->GetName() : TEXT("None"), PooledBulletClass ? *PooledBulletClass->GetName() : TEXT("None"));

	if (PooledBulletClass)
	{
		FVector ShootDirection = AimLocation - BulletSpawnLocation;
		AMyPlayer* PlayerOwner = Cast<AMyPlayer>(GetOwner());
		Params.SpawnDirection = ShootDirection;
		Params.SpawnLocation = BulletSpawnLocation;
		FRotator SpawnRotation = ShootDirection.Rotation();
		Params.SpawnRotation = SpawnRotation;
		auto SpawnedBullet = Pool->AcquireBullet(PooledBulletClass,Params);
		if (!SpawnedBullet || !Cast<APooledBullet>(SpawnedBullet))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to acquire bullet from pool in MyShootingComponent!"));
			return;
		}
		if (PlayerOwner)
		{
			PlayerOwner->ApplyRecoil(GenerateRecoilImpulse());
		}
		/*CurrentSpreadAngle = FMath::Clamp(CurrentSpreadAngle + SpreadIncreasePerShot, 0.0f, MaxSpreadAngle);
		CurrentVerticalKick = FMath::Clamp(CurrentVerticalKick + VerticalKickPerShot, 0.0f, MaxVerticalKick);
		if (PlayerOwner) 
		{
			if (APlayerController* PC = Cast<APlayerController>(PlayerOwner->GetController()))
			{
				PC->AddPitchInput(-VerticalKickPerShot); // 上抬枪口
			}
		}*/
	}
}

/*FVector UMyShootingComponent::ApplySpreadToDirection(const FVector& Direction, bool bIsAiming) const
{
	const float BaseSpread = bIsAiming ? AimSpreadAngle : HipFireSpreadAngle;
	const float FinalSpreadDeg = FMath::Clamp(BaseSpread + CurrentSpreadAngle, 0.0f, MaxSpreadAngle);
	const float ConeHalfAngleRad = FMath::DegreesToRadians(FinalSpreadDeg);

	// 2) 锥形随机散布
	FVector OutDir = FMath::VRandCone(Direction.GetSafeNormal(), ConeHalfAngleRad);

	// 3) 持续上抬 + 少量水平抖动
	FRotator R = OutDir.Rotation();
	R.Pitch -= CurrentVerticalKick; // 若方向反了，改成 +=
	const float AimMul = bIsAiming ? 0.5f : 1.0f;
	R.Yaw += FMath::FRandRange(-HorizontalJitter, HorizontalJitter) * AimMul;

	return R.Vector().GetSafeNormal();
}*/

/*void UMyShootingComponent::PlayShotCameraShake(bool bIsAiming) const
{
	if (!ShotCameraShakeClass) return;
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;
	APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PlayerController||!PlayerController->PlayerCameraManager) return;
	
	const float Scale = bIsAiming ? CameraShakeScaleAim : ShotCameraShakeScale;
	PlayerController->PlayerCameraManager->StopAllInstancesOfCameraShake(ShotCameraShakeClass, false);
	PlayerController->PlayerCameraManager->StartCameraShake(ShotCameraShakeClass, Scale);
}*/

void UMyShootingComponent::StartShooting()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot start shooting: World context is null in MyShootingComponent!"));
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, this, &UMyShootingComponent::ShootBall, FireDelay, true);
}

void UMyShootingComponent::StopShooting()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot stop shooting: World context is null in MyShootingComponent!"));
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
}

void UMyShootingComponent::UpdateAim(float DeltaTime)
{
	if (!GetOwner()) return;
	AController* OwnerController = Cast<AController>(GetOwner()->GetInstigatorController());
	if (!OwnerController) return;
	FVector AimDirection = AimLocation - GetOwner()->GetActorLocation();
	//AimDirection.Z = 0.0f;
	if (AimDirection.IsNearlyZero()) return;
	FRotator DesiredRotation = AimDirection.Rotation();
	FRotator CurrentRotation = OwnerController->GetControlRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaTime, AimingSpeed);
	GetOwner()->SetActorRotation(NewRotation);
}

FAngularImpulse UMyShootingComponent::GenerateRecoilImpulse() const
{
	FAngularImpulse RecoilImpulse;
	RecoilImpulse.Torque.X = SpreadConfig.PitchSpreadAngle;
	RecoilImpulse.Torque.Y = GetImpluseAxisValue(SpreadConfig.Yaw);
	RecoilImpulse.Torque.Z = GetImpluseAxisValue(SpreadConfig.Roll);
	return RecoilImpulse;
}

void UMyShootingComponent::UpdateAimPoint(float DeltaTime)
{
	FVector ShootDirection = AimLocation - BulletSpawnLocation;
	FHitResult HitResult;
	bool bHit = false;
	bHit = GetWorld()->LineTraceSingleByChannel(HitResult, BulletSpawnLocation, BulletSpawnLocation + ShootDirection * 3000.0f, ECC_Visibility);
	if (!bHit)
	{
		AimPoint = BulletSpawnLocation + ShootDirection * 3000.0f;
	}
	else
	{
		AimPoint = HitResult.ImpactPoint;
	}
}


// Called every frame
void UMyShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//CurrentSpreadAngle = FMath::FInterpTo(CurrentSpreadAngle, 0.0f, DeltaTime, SpreadRecoveryRate);
	//CurrentVerticalKick = FMath::FInterpTo(CurrentVerticalKick, 0.0f, DeltaTime, VerticalRecoverSpeed);
	UpdateAimPoint(DeltaTime);
	// ...
}

