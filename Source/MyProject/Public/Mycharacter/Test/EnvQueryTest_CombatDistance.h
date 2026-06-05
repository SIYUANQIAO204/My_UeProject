// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvQueryTest_CombatDistance.generated.h"


/**
 * 
 */
UCLASS()
class MYPROJECT_API UEnvQueryTest_CombatDistance : public UEnvQueryTest
{
	GENERATED_BODY()
	
public:
	UEnvQueryTest_CombatDistance();

protected:
	virtual void RunTest(
		FEnvQueryInstance& QueryInstance
	)const override;

	UPROPERTY(EditDefaultsOnly, Category = "CombatDistance")
	FAIDataProviderFloatValue DesiredDistance;

	UPROPERTY(EditDefaultsOnly, Category = "CombatDistance")
	FAIDataProviderFloatValue DistanceTolerance;

	UPROPERTY(EditDefaultsOnly, Category = "CombatDistance")
	FAIDataProviderFloatValue MinDistance;

	UPROPERTY(EditDefaultsOnly, Category = "CombatDistance")
	FAIDataProviderFloatValue MaxDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Target")
	TSubclassOf<UEnvQueryContext> TargetContext;

};
