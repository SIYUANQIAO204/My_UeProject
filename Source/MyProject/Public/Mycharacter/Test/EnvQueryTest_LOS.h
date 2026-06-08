// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_LOS.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UEnvQueryTest_LOS : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_LOS();
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Context")
	TSubclassOf<UEnvQueryContext> TargetContext;
	

	
	UPROPERTY(EditDefaultsOnly, Category = "TraceChannel")
	TEnumAsByte<ECollisionChannel> TraceChannel;
};
