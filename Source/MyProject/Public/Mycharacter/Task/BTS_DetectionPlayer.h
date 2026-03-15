// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_DetectionPlayer.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MYPROJECT_API UBTS_DetectionPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_DetectionPlayer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
