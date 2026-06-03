// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include  "Mycharacter/SightComponent.h"
#include "Projectile/BallProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/MyShootingComponent.h"
#include "Component/MyEnemyPatrolComponent.h"
#include "Component/HealthComponent.h"
#include "Component/MYEnemyMovingComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SightComponent = CreateDefaultSubobject<USightComponent>(TEXT("SightComponent"));
	SightComponent->SetupAttachment(RootComponent);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel2);
	ShootingComponent = CreateDefaultSubobject<UMyShootingComponent>(TEXT("ShootingComponent"));
	PatrolComponent = CreateDefaultSubobject<UMyEnemyPatrolComponent>(TEXT("PatrolComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	EnemyMovingComponent = CreateDefaultSubobject<UMYEnemyMovingComponent>(TEXT("MYEnemyMovingComponent"));

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitTargertCharacter();
	
}

void AEnemy::InitTargertCharacter()
{
	
	TargetCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	SightComponent->SetTargetActor(TargetCharacter);
}


// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::MoveToTarget(FVector TargetLocation)
{
	if (!EnemyMovingComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyMovingComponent is null"));
		return;
	}
	EnemyMovingComponent->SetTargetLocation(TargetLocation);
}

ETeam AEnemy::GetTeam_Implementation() const
{
	return Team;
}

void AEnemy::Death_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Death"));
	Destroy();
	return;
}

void AEnemy::Damage_Implementation(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Damage: %f"), DamageAmount);
	HealthComponent->TakeDamage(DamageAmount);
	return;
}

