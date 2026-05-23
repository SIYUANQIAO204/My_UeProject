// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjcetPool/MyBulletObjectPool.h"
#include "ObjectPool/PooledActor.h"
#include "Engine/World.h"
#include "Interface/IPoolable.h"

void UMyBulletObjectPool::Initialize(UWorld* GameWorld, TSubclassOf<APooledActor> InBulletClass, int32 InitialSize)
{
	BulletClass = InBulletClass;
	World = GameWorld;
	DefaultPoolExpandSize = 0;
	ExpandPool(InitialSize);
	
}

APooledActor* UMyBulletObjectPool::AcquireBullet()
{
	if (InactiveBullets.Num() == 0)
	{
		ExpandPool(DefaultPoolExpandSize);
	}
	APooledActor* Bullet = InactiveBullets.Pop();
	if (Bullet->Implements<UIPoolable>())
	{
		IIPoolable::Execute_OnAcquireFromPool(Bullet);
	}
	ActiveBullets.Add(Bullet);
	return Bullet;
}

void UMyBulletObjectPool::ReleaseBullet(APooledActor* Bullet)
{
	if (Bullet)
	{
		if (Bullet->Implements<UIPoolable>())
		{
			IIPoolable::Execute_OnReleaseToPool(Bullet);
		}
		ActiveBullets.RemoveSwap(Bullet);
		InactiveBullets.Add(Bullet);
	}
}

void UMyBulletObjectPool::PoolTick(float DeltaTime)
{
	for (int32 i = ActiveBullets.Num() - 1; i >= 0; --i)
	{
		APooledActor* Bullet = ActiveBullets[i];
		if (Bullet)
		{
			if (Bullet->Implements<UIPoolable>())
			{
				IIPoolable::Execute_PoolTick(Bullet, DeltaTime);
				if (Bullet->IsFinished())
				{
					ReleaseBullet(Bullet);
				}
			}
			
		}
	}
}

void UMyBulletObjectPool::ExpandPool(int32 AdditionalSize)
{
	for (int32 i = 0; i < AdditionalSize; ++i)
	{
		APooledActor* Bullet = World->SpawnActor<APooledActor>(BulletClass);
		Bullet->SetPool(this);
		if (Bullet->Implements<UIPoolable>())
		{
			IIPoolable::Execute_OnReleaseToPool(Bullet);
		}
		BulletPool.Add(Bullet);
		InactiveBullets.Add(Bullet);
	}
	DefaultPoolExpandSize += AdditionalSize;
}

