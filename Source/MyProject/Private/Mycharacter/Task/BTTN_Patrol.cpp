// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Task/BTTN_Patrol.h"
#include "Mycharacter/MyShootAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Character.h"
#include "Component/MyEnemyPatrolComponent.h"

UBTTN_Patrol::UBTTN_Patrol()
{
	NodeName = TEXT("Patrol");
}

EBTNodeResult::Type UBTTN_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyShootAIController* MyController = Cast<AMyShootAIController>(OwnerComp.GetAIOwner());
	if(MyController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::InProgress;
}