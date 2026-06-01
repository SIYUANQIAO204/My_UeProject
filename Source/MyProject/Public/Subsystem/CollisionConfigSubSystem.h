// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "enum/CollisionChannelType.h"
#include "enum/CollisionTraceChannelType.h"
#include "CollisionConfigSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UCollisionConfigSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UCollisionConfigSubSystem();

	void Initialize(FSubsystemCollectionBase& Collision) override;
	
	EObjectTypeQuery GetQurey(ECollisionObjectChannelType ObjectType) const;

private:

	TMap < ECollisionObjectChannelType, EObjectTypeQuery> ObjectChannelMap;

	
};
