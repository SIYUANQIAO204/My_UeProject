// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollisionChannelType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECollisionObjectChannelType : uint8
{
    Default,
    Pawn,
    WorldStatic,
    WorldDynamic,
    EnemyBullet,
    Enemy,
    Player, 
	PlayerBullet,
	Vehicle,
	Destructible
};