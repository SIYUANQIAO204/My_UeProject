// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MyShootingComponent.h"
#include "Projectile/BallProjectile.h"
#include "Interface/TeamInterface.h"
#include "MyPlayer.h"

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
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		if (PlayerOwner)
		{
			SpawnLocation = PlayerOwner->GetMuzzleLocation();
			ShootDirection = PlayerOwner->GetMuzzleForwardVector();
			if (PlayerOwner->GetIsAiming())
			{
				ShootDirection = PlayerOwner->GetAimDirection();
			}
		}
		ShootDirection = ShootDirection.GetSafeNormal();
		FRotator SpawnRotation = ShootDirection.Rotation();
		FTransform SpawnTransform(SpawnRotation, SpawnLocation);
		SpawnParams.Owner = GetOwner();
		ABallProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<ABallProjectile>(BallProjectileClass, SpawnTransform, GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
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
		UE_LOG(LogTemp, Warning, TEXT("Shooting ball from %s with direction %s"), *SpawnLocation.ToString(), *ShootDirection.ToString());
		SpawnedProjectile->FinishSpawning(SpawnTransform);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BallProjectileClass is not set in MyShootingComponent!"));
	}

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

	// ...
}

