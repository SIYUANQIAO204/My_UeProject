// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_MyShoot.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTTaskNode_MyShoot : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_MyShoot();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	float ShoootDelay = 3.0f;
	
};
