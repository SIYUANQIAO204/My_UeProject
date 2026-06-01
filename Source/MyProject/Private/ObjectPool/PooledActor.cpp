// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool/PooledActor.h"
#include "ObjectPool/MyBulletObjectPoolSubSystem.h"

// Sets default values
APooledActor::APooledActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void APooledActor::OnAcquireFromPool_Implementation(FPoolActorPrama Params)
{
	SetActorLocation(Params.SpawnLocation);
	SetActorRotation(Params.SpawnRotation);
	bIsFinished = false;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void APooledActor::OnReleaseToPool_Implementation()
{
	SetActorLocation(FVector(0,0,-1000.0));
	bIsFinished = true;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void APooledActor::PoolTick_Implementation(float DeltaTime)
{
}

void APooledActor::SetPool(FBulletPoolUnit* InPool)
{
	Pool = MakeShared<FBulletPoolUnit>(*InPool);
}

bool APooledActor::IsFinished_Implementation() const
{
	return bIsFinished;
}



// Called when the game starts or when spawned
void APooledActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APooledActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

