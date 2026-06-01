// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool/MyBulletObjectPoolSubSystem.h"
#include "ObjectPool/PooledActor.h"
#include "Interface/IPoolable.h"

void UMyBulletObjectPoolSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("MyBulletObjectPoolSubSystem initialized"));
}

void UMyBulletObjectPoolSubSystem::Tick(float DeltaTime)
{
	UE_LOG(LogTemp, Log, TEXT("MyBulletObjectPoolSubSystem::Tick called. Pools=%d"), BulletPoolMap.Num());
	for(auto& Pair : BulletPoolMap)
	{
		FBulletPoolUnit& Pool = Pair.Value;
		UE_LOG(LogTemp, Verbose, TEXT("Pool %s Active=%d Inactive=%d"), * (Pool.BulletClass ? Pool.BulletClass->GetName() : TEXT("None")), Pool.ActiveBullets.Num(), Pool.InactiveBullets.Num());
		for (APooledActor* Bullet : Pool.ActiveBullets)
		{
			if (Bullet && Bullet->Implements<UIPoolable>())
			{
				IIPoolable::Execute_PoolTick(Bullet, DeltaTime);
			}
		}
	}
}

APooledActor* UMyBulletObjectPoolSubSystem::AcquireBullet(TSubclassOf<APooledActor> BulletClass, FPoolActorPrama Params)
{
	UE_LOG(LogTemp, Log, TEXT("AcquireBullet called for class %s"), * (BulletClass ? BulletClass->GetName() : TEXT("None")) );
	FBulletPoolUnit* PoolUnit = nullptr;
	PoolUnit =	BulletPoolMap.Find(BulletClass);
	if (PoolUnit == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("No existing pool for %s, creating new pool unit."), * (BulletClass ? BulletClass->GetName() : TEXT("None")) );
		PoolUnit = &(this ->CreatePoolUnit(BulletClass));
	}
	if (PoolUnit->InactiveBullets.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("InactiveBullets empty for %s, expanding pool. Current PoolSize=%d"), * (BulletClass ? BulletClass->GetName() : TEXT("None")), PoolUnit->PoolSize);
		this -> ExpandPool(*PoolUnit, PoolUnit->PoolSize);
	}
	if (PoolUnit->InactiveBullets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AcquireBullet: still no inactive bullets for %s after ExpandPool!"), * (BulletClass ? BulletClass->GetName() : TEXT("None")) );
		return nullptr;
	}
	APooledActor* Bullet = PoolUnit->InactiveBullets.Pop();
	UE_LOG(LogTemp, Log, TEXT("AcquireBullet: popped bullet %s from InactiveBullets"), Bullet ? *Bullet->GetName() : TEXT("Null"));
	if (Bullet == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AcquireBullet: popped null from InactiveBullets for %s"), * (BulletClass ? BulletClass->GetName() : TEXT("None")) );
		return nullptr;
	}
	if (Bullet->Implements<UIPoolable>())
	{
		IIPoolable::Execute_OnAcquireFromPool(Bullet, Params);
		PoolUnit->ActiveBullets.Add(Bullet);
		UE_LOG(LogTemp, Log, TEXT("AcquireBullet: added bullet %s to ActiveBullets, ActiveCount=%d"), *Bullet->GetName(), PoolUnit->ActiveBullets.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AcquireBullet: bullet %s does not implement UIPoolable"), *Bullet->GetName());
	}
	return Bullet;
}

FBulletPoolUnit& UMyBulletObjectPoolSubSystem::CreatePoolUnit(TSubclassOf<APooledActor> BulletClass)
{
	UE_LOG(LogTemp, Log, TEXT("CreatePoolUnit called for %s"), * (BulletClass ? BulletClass->GetName() : TEXT("None")) );
	FBulletPoolUnit NewPoolUnit;
	NewPoolUnit.ActiveBullets = {};
	NewPoolUnit.BulletClass = BulletClass;
	this->ExpandPool(NewPoolUnit, 64);
	FBulletPoolUnit& Added = BulletPoolMap.Add(BulletClass, NewPoolUnit);
	UE_LOG(LogTemp, Log, TEXT("CreatePoolUnit: pool created for %s with PoolSize=%d InactiveCount=%d"), * (BulletClass ? BulletClass->GetName() : TEXT("None")), Added.PoolSize, Added.InactiveBullets.Num());
	return Added;
}

void UMyBulletObjectPoolSubSystem::ReleaseBullet(APooledActor* Bullet)
{
	if (Bullet->Implements<UIPoolable>())
	{
		IIPoolable::Execute_OnReleaseToPool(Bullet);
		for (auto& Pair : BulletPoolMap)
		{
			FBulletPoolUnit& Pool = Pair.Value;
			if (Pool.ActiveBullets.Remove(Bullet) > 0)
			{
				Pool.InactiveBullets.Add(Bullet);
				break;
			}
		}
	}
}

void UMyBulletObjectPoolSubSystem::ExpandPool(FBulletPoolUnit& Pool, int32 count)
{
	UE_LOG(LogTemp, Log, TEXT("ExpandPool called for class %s, count=%d"), * (Pool.BulletClass ? Pool.BulletClass->GetName() : TEXT("None")), count);
	int32 Spawned = 0;
	for (int i = 0; i < count; i++)
	{
		APooledActor* NewBullet = GetWorld()->SpawnActor<APooledActor>(Pool.BulletClass);
		if (NewBullet)
		{
			NewBullet->SetPool(&Pool);
			NewBullet->SetActorEnableCollision(false);
			NewBullet->SetActorHiddenInGame(true);
			NewBullet->SetActorTickEnabled(false);
			Pool.InactiveBullets.Add(NewBullet);
			Spawned++;
			UE_LOG(LogTemp, Verbose, TEXT("ExpandPool: Spawned bullet %s"), *NewBullet->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ExpandPool: Failed to spawn bullet for class %s"), * (Pool.BulletClass ? Pool.BulletClass->GetName() : TEXT("None")) );
		}
	}
	Pool.PoolSize += count;
	UE_LOG(LogTemp, Log, TEXT("ExpandPool finished for %s: Spawned=%d PoolSize=%d Inactive=%d"), * (Pool.BulletClass ? Pool.BulletClass->GetName() : TEXT("None")), Spawned, Pool.PoolSize, Pool.InactiveBullets.Num());
}

