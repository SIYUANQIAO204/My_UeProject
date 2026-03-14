// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_WhetherDetected.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTD_WhetherDetected : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTD_WhetherDetected();
	
protected:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
