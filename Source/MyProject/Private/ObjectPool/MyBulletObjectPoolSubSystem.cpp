// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool/MyBulletObjectPoolSubSystem.h"
#include "ObjcetPool/MyBulletObjectPool.h"

void UMyBulletObjectPoolSubSystem::Tick(float DeltaTime)
{
	for(UMyBulletObjectPool* Pool : BulletPools)
	{
		if (Pool)
		{
			Pool->PoolTick(DeltaTime);
		}
	}
}