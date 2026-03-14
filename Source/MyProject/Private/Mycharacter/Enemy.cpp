// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include  "Mycharacter/SightComponent.h"
#include "Projectile/BallProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/MyShootingComponent.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SightComponent = CreateDefaultSubobject<USightComponent>(TEXT("SightComponent"));
	SightComponent->SetupAttachment(RootComponent);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel2);
	ShootingComponent = CreateDefaultSubobject<UMyShootingComponent>(TEXT("ShootingComponent"));

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitTargertCharacter();
	
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

void AEnemy::InitTargertCharacter()
{
	
	TargetCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	SightComponent->SetTargetActor(TargetCharacter);
}

/*void AEnemy::ShootBall()
{
	/*if (BallProjectileClass)
	{
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 40.0f;
		FRotator SpawnRotation = GetActorRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		GetWorld()->SpawnActor<ABallProjectile>(BallProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		//FTransform SpawnTransform(SpawnRotation, SpawnLocation);
		//ABallProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<ABallProjectile>(BallProjectileClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		//SpawnedProjectile->FinishSpawning(SpawnTransform);

	}
	ShootingComponent->StartShooting();
}*/

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
			FVector Direction = TargetCharacter->GetActorLocation() - GetActorLocation();
			Direction.Z = 0; // 如果只想水平旋转
			FRotator NewRotation = Direction.Rotation();
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), NewRotation, 0.5f, 1.f));
			GetWorldTimerManager().SetTimer(ShootTimerHandle, ShootingComponent.Get(), &UMyShootingComponent::StartShooting, ShootInterval, false);

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy lost sight of the player!"));
			ShootingComponent->StopShooting();
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

