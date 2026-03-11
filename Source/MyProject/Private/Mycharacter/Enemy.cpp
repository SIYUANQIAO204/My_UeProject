// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	TargetCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

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

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bCanSeePlayer = LineTraceActor(TargetCharacter);
	if (bCanSeePlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy can see the player!"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy can't see the player!"));
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

