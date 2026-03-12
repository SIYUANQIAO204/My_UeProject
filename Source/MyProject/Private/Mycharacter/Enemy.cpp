// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include  "Mycharacter/SightComponent.h"
#include "Projectile/BallProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SightComponent = CreateDefaultSubobject<USightComponent>(TEXT("Sight Component"));
	SightComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	TargetCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	SightComponent->SetTargetActor(TargetCharacter);
}
/*
bool AEnemy::LineTraceActor(AActor* TargetActor)
{
	if(TargetActor == nullptr)
	{
		return false;
	}
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = TargetActor->GetActorLocation();
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(TargetActor);
	if (CanSeeActor(TargetActor, Start, End))
	{
		FRotator DirectRotation = (End - Start).Rotation();
		SetActorRotation(DirectRotation);
		return true;
	}
	return false;
}
*/
/*
bool AEnemy::CanSeeActor(const AActor* TargetActor, FVector Start, FVector End) const
{
	if (TargetActor == nullptr)
	{
		return false;
	}
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(TargetActor);
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	return !HitResult.bBlockingHit;
}
*/

void AEnemy::ShootBall()
{
	if (BallProjectileClass)
	{
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 40.0f;
		FRotator SpawnRotation = GetActorRotation();
		/*FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;*/
		//GetWorld()->SpawnActor<ABallProjectile>(BallProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		FTransform SpawnTransform(SpawnRotation, SpawnLocation);
		ABallProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<ABallProjectile>(BallProjectileClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		SpawnedProjectile->FinishSpawning(SpawnTransform);

	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bPreviousCanSeePlayer = bCanSeePlayer;
	bCanSeePlayer = SightComponent->IsTargetInSight();
	if (bCanSeePlayer != bPreviousCanSeePlayer)
	{
		if (bCanSeePlayer)
		{
			GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &AEnemy::ShootBall, FireDelay, true, ShootInterval);

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy lost sight of the player!"));
			GetWorldTimerManager().ClearTimer(ShootTimerHandle);
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

