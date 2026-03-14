// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Task/BTTaskNode_MyShoot.h"
#include "Mycharacter/MyShootAIController.h"
#include "GameFramework/Character.h"
#include "Mycharacter/Enemy.h"
#include "Component/MyShootingComponent.h"

UBTTaskNode_MyShoot::UBTTaskNode_MyShoot()
{
	NodeName = TEXT("MyShoot");
}

EBTNodeResult::Type UBTTaskNode_MyShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyShootAIController* MyController = Cast<AMyShootAIController>(OwnerComp.GetAIOwner());
	if (MyController)
	{
		AEnemy* MyPawn = Cast<AEnemy>(MyController->GetCharacter());
		if (MyPawn)
		{
			MyPawn->ShootingComponent->StartShooting();
			return EBTNodeResult::InProgress;
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTaskNode_MyShoot::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AMyShootAIController* MyController = Cast<AMyShootAIController>(OwnerComp.GetAIOwner());
	if (MyController)
	{
		AEnemy* MyPawn = Cast<AEnemy>(MyController->GetCharacter());
		if (MyPawn)
		{
			if (!MyPawn->ShootingComponent)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return;
			}
			else
			{
				return;
			}
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return;
}
