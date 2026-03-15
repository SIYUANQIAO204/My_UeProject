// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTN_EnemyPatrol.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MYPROJECT_API UBTTN_EnemyPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTN_EnemyPatrol();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Config")
	float AcceptanceRadius = 200.0f;
};
