// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETeam : uint8
{
    Player   UMETA(DisplayName = "Player"),
    Enemy    UMETA(DisplayName = "Enemy"),
    Neutral  UMETA(DisplayName = "Neutral")
};