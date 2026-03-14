// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MyShootingComponent.h"
#include "Projectile/BallProjectile.h"


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
		FVector SpawnLocation = GetOwner()-> GetActorLocation() + GetOwner()->GetActorForwardVector() * 40.0f;
		FRotator SpawnRotation = GetOwner()->GetActorRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		GetWorld()->SpawnActor<ABallProjectile>(BallProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		//FTransform SpawnTransform(SpawnRotation, SpawnLocation);
		//ABallProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<ABallProjectile>(BallProjectileClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		//SpawnedProjectile->FinishSpawning(SpawnTransform);

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

