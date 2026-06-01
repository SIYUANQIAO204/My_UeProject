// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollisionTraceChannelType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECollisionTraceChannelPresetType : uint8
{
    Default,
	BlockAll,
	NoCollision,
	Visibility,
	Enemy,
	Player,
	EnemyBullet,
	PlayerBullet,
	Pawn,
	Spector
};
