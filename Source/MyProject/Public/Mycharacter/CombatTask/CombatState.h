// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "CombatState.generated.h"

/**
 * 
 */
UENUM()
enum class ECombatState : uint8
{
	FindPosition,
	Moving,
	Shooting,
	Reposition,
	Finished
	
};
