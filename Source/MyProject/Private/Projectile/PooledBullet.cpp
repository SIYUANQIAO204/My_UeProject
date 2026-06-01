// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/PooledBullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyPlayer.h"
#include "Interface/TeamInterface.h"
#include "ObjectPool/MyBulletObjectPoolSubSystem.h"
#include "Interface/HealthInterface.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Component/HealthComponent.h"

APooledBullet::APooledBullet()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APooledBullet::OnAcquireFromPool_Implementation(FPoolActorPrama Params)
{
	UE_LOG(LogTemp, Log, TEXT("APooledBullet::OnAcquireFromPool_Implementation called for %s"), *GetName());
	Super::OnAcquireFromPool_Implementation(Params);
	ActivateBullet(Params);
	if (!BulletData)
	{
		UE_LOG(LogTemp, Warning, TEXT("APooledBullet::OnAcquireFromPool_Implementation: BulletData is null after ActivateBullet for %s"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("APooledBullet::OnAcquireFromPool_Implementation: Acquired bullet %s with Owner=%s InitialSpeed=%f"), *GetName(), BulletData->BulletOwner ? *BulletData->BulletOwner->GetName() : TEXT("None"), BulletData->InitialSpeed);
	}
}

void APooledBullet::PoolTick_Implementation(float DeltaTime)
{
	if (bIsFinished) return;
	if (!BulletData)
	{
		UE_LOG(LogTemp, Warning, TEXT("PoolTick: BulletData is null on %s, releasing."), *GetName());
		ReleaseBullet();
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("PoolTick: Bullet %s at Location=%s Velocity=%s Life=%f"), *GetName(), *Location.ToString(), *Velocity.ToString(), CurrentLifeTime);
	FVector GravityForce = FVector(0.f, 0.f, -980.f)* BulletData->Mass;
	FVector ResistanceForce = -Velocity * BulletData->Resistance;
	FVector TotalForce = GravityForce + ResistanceForce;
	FVector Acceleration = TotalForce / BulletData->Mass;
	Velocity += Acceleration * DeltaTime;
	FVector StartLocation = Location;
	FVector EndLocation = Location + Velocity * DeltaTime;
	CurrentSpeed = Velocity.Size();
	FHitResult HitResult;

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult, 
		StartLocation, 
		EndLocation, 
		FQuat::Identity, 
		BulletData->CollisionChannel, 
		CollisionShape,
		CollisionParams
	);
	if (bHit)
	{
		OnBulletHit(HitResult.GetActor());

	}
	if (bIsFinished)
	{
		ReleaseBullet();
		return;
	}
	SetActorLocation(EndLocation);
	Location = EndLocation;
	UE_LOG(LogTemp, Log, TEXT("PoolTick: Bullet %s moved to %s"), *GetName(), *EndLocation.ToString());
	CurrentLifeTime += DeltaTime;
	if (CurrentLifeTime >= BulletData->LifeTime)
	{
		bIsFinished = true;
		ReleaseBullet();
	}
}

void APooledBullet::ActivateBullet(FPoolActorPrama Params)
{
	UE_LOG(LogTemp, Log, TEXT("ActivateBullet called on %s with SpawnLocation=%s SpawnDirection=%s"), *GetName(), *Params.SpawnLocation.ToString(), *Params.SpawnDirection.ToString());
	UBulletData* IncomingBulletData = Cast<UBulletData>(Params.ExtraData);
	if (!IncomingBulletData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActivateBullet failed: Params.ExtraData is not UBulletData on %s"), *GetName());
		bIsFinished = true;
		return;
	}

	BulletData = IncomingBulletData;
	Location = Params.SpawnLocation;
	Direction = Params.SpawnDirection.GetSafeNormal();
	Velocity = Direction * BulletData->InitialSpeed;
	CurrentLifeTime = 0.0f;
	CollisionParams.ClearIgnoredActors();
	CollisionParams.AddIgnoredActor(this);
	if (BulletData->BulletOwner)
	{
		CollisionParams.AddIgnoredActor(BulletData->BulletOwner);
	}
	CollisionShape = FCollisionShape::MakeSphere(BulletData->Radius);
	SetActorLocation(Location);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	UE_LOG(LogTemp, Log, TEXT("ActivateBullet: %s initialized. Owner=%s InitialSpeed=%f Radius=%f"), *GetName(), BulletData->BulletOwner ? *BulletData->BulletOwner->GetName() : TEXT("None"), BulletData->InitialSpeed, BulletData->Radius);
}

void APooledBullet::ReleaseBullet()
{
	UE_LOG(LogTemp, Log, TEXT("ReleaseBullet called on %s"), *GetName());
	SetActorLocation(FVector(0,0,-1000.0));
	CollisionParams.ClearIgnoredActors();
	if(this->Implements<UIPoolable>())
	{
		IIPoolable::Execute_OnReleaseToPool(this);
	}
}

void APooledBullet::OnBulletHit(AActor* OtherActor)
{
	if (!OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnBulletHit called with null OtherActor on %s"), *GetName());
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Bullet hit: %s by %s"), *GetName(), *OtherActor->GetName());
	if (!OtherActor->Implements<UTeamInterface>())
	{
		bIsFinished = true;
		return;
	}

	ETeam OtherTeam = ETeam::Neutral;

	if (OtherActor->Implements<UTeamInterface>())
	{
		OtherTeam = ITeamInterface::Execute_GetTeam(OtherActor);
	}

	if (BulletData->Team == OtherTeam)
	{
		return;
	}
	else {
		if (OtherActor->Implements<UHealthInterface>())
		{
			bIsFinished = true;
			IHealthInterface::Execute_Damage(OtherActor, BulletData->Damage * (CurrentSpeed / BulletData->InitialSpeed));
		}
		else
		{
			bIsFinished = true;
			return;
		}
		bIsFinished = true;
	}
}
