// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MyShootingComponent.h"
#include "Projectile/BallProjectile.h"
#include "Interface/TeamInterface.h"
#include "MyPlayer.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"


// Sets default values for this component's properties
UMyShootingComponent::UMyShootingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyShootingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UMyShootingComponent::ShootBall()
{
	
	if (BallProjectileClass)
	{
		FVector SpawnLocation = GetOwner()->GetActorLocation()+GetOwner()->GetActorForwardVector() * 40.0f;
		FVector ShootDirection = GetOwner()->GetActorForwardVector();
		AMyPlayer* PlayerOwner = Cast<AMyPlayer>(GetOwner());
		const bool bIsAiming = PlayerOwner ? PlayerOwner->GetIsAiming() : false;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		if (PlayerOwner)
		{
			SpawnLocation = PlayerOwner->GetMuzzleLocation();
			if (bIsAiming)
			{
				ShootDirection = PlayerOwner->GetAimDirection();
			}
			else if (APlayerController* PC = Cast<APlayerController>(PlayerOwner->GetController()))
			{
				ShootDirection = PC->PlayerCameraManager
					? PC->PlayerCameraManager->GetCameraRotation().Vector()
					: PlayerOwner->GetActorForwardVector();
			}
			else
			{
				ShootDirection = PlayerOwner->GetActorForwardVector();
			}
		}
		ShootDirection = ApplySpreadToDirection(ShootDirection, bIsAiming).GetSafeNormal();

		FRotator SpawnRotation = ShootDirection.Rotation();
		FTransform SpawnTransform(SpawnRotation, SpawnLocation);
		SpawnParams.Owner = GetOwner();
		ABallProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<ABallProjectile>(
			BallProjectileClass, 
			SpawnTransform, 
			GetOwner(), 
			nullptr, 
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (!SpawnedProjectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn projectile in MyShootingComponent!"));
			return;
		}
		if (!GetOwner() || !GetOwner()->Implements<UTeamInterface>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner does not implement ITeamInterface in MyShootingComponent!"));
			return;
		}
		SpawnedProjectile->InitBullet(GetOwner(), ITeamInterface::Execute_GetTeam(GetOwner()));
		SpawnedProjectile->InitVelocity(ShootDirection);
		//UE_LOG(LogTemp, Warning, TEXT("Shooting ball from %s with direction %s"), *SpawnLocation.ToString(), *ShootDirection.ToString());
		SpawnedProjectile->FinishSpawning(SpawnTransform);

		CurrentSpreadAngle = FMath::Clamp(CurrentSpreadAngle + SpreadIncreasePerShot, 0.0f, MaxSpreadAngle);
		CurrentVerticalKick = FMath::Clamp(CurrentVerticalKick + VerticalKickPerShot, 0.0f, MaxVerticalKick);
		if (PlayerOwner) 
		{
			if (APlayerController* PC = Cast<APlayerController>(PlayerOwner->GetController()))
			{
				PC->AddPitchInput(-VerticalKickPerShot); // 上抬枪口
			}
			PlayShotCameraShake(bIsAiming);
		}
	}
}

FVector UMyShootingComponent::ApplySpreadToDirection(const FVector& Direction, bool bIsAiming) const
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
}

void UMyShootingComponent::PlayShotCameraShake(bool bIsAiming) const
{
	if (!ShotCameraShakeClass) return;
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;
	APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PlayerController||!PlayerController->PlayerCameraManager) return;
	
	const float Scale = bIsAiming ? CameraShakeScaleAim : ShotCameraShakeScale;
	PlayerController->PlayerCameraManager->StopAllInstancesOfCameraShake(ShotCameraShakeClass, false);
	PlayerController->PlayerCameraManager->StartCameraShake(ShotCameraShakeClass, Scale);
}

void UMyShootingComponent::StartShooting()
{
	GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, this, &UMyShootingComponent::ShootBall, FireDelay, true);
}

void UMyShootingComponent::StopShooting()
{
	GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
}

// Called every frame
void UMyShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurrentSpreadAngle = FMath::FInterpTo(CurrentSpreadAngle, 0.0f, DeltaTime, SpreadRecoveryRate);
	CurrentVerticalKick = FMath::FInterpTo(CurrentVerticalKick, 0.0f, DeltaTime, VerticalRecoverSpeed);
	// ...
}

