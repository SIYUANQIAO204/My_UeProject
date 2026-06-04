// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_ProjectedPoints.h"
#include "DataProviders/AIDataProvider.h"
#include "CombatDistanceGenerator.generated.h"


/**
 * 
 */
UCLASS()
class MYPROJECT_API UCombatDistanceGenerator : public UEnvQueryGenerator_ProjectedPoints
{
	GENERATED_BODY()
	
public:

	UCombatDistanceGenerator();

	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "CombatDistance")
	FAIDataProviderFloatValue OuterRadius;

	UPROPERTY(EditDefaultsOnly, Category = "CombatDistance")
	FAIDataProviderFloatValue InnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "CombatDistance")
	FAIDataProviderIntValue NumberOfRings;

	UPROPERTY(EditDefaultsOnly, Category = "CombatDistance")
	FAIDataProviderIntValue NumberOfPoints;

	UPROPERTY(EditDefaultsOnly, Category = "CombatDistance")
	FAIDataProviderFloatValue AngleOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Context")
	TSubclassOf<UEnvQueryContext> PlayerContext;
};
