// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_UpdateSplinePoint.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTS_UpdateSplinePoint : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTS_UpdateSplinePoint();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
