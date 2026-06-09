// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Task/BTTN_EnemyPatrol.h"
#include "Mycharacter/MyShootAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Component/MyEnemyPatrolComponent.h"
#include "Component/MYEnemyMovingComponent.h"
#include "Mycharacter/Enemy.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"

UBTTN_EnemyPatrol::UBTTN_EnemyPatrol()
{
	NodeName = "Enemy Patrol";
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTN_EnemyPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyShootAIController* AIController = Cast<AMyShootAIController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		APawn* ControlledPawn = AIController->GetPawn();
		if (ControlledPawn)
		{
			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			if (BlackboardComp)
			{
				FVector PatrolPoint = BlackboardComp->GetValueAsVector("PatrolPoint");
				AEnemy* Enemy = Cast<AEnemy>(AIController->GetPawn());
				if (Enemy)
				{
					///UE_LOG(LogTemp, Warning, TEXT("Start Enemy Patrol task to point: %s"), *PatrolPoint.ToString());
					Enemy->MoveToTarget(PatrolPoint);
				}
				return EBTNodeResult::InProgress;
			}
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Failed to execute Enemy Patrol task"));
	return EBTNodeResult::Failed;
}

void UBTTN_EnemyPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AMyShootAIController* AIController = Cast<AMyShootAIController>(OwnerComp.GetAIOwner());
	if (AIController)
	{

		AEnemy* Enemy = Cast<AEnemy>(AIController->GetPawn());
		if (Enemy)
		{
			UMYEnemyMovingComponent* MovingComponent = Enemy->FindComponentByClass<UMYEnemyMovingComponent>();
			if (MovingComponent)
			{
				if (!MovingComponent->IsMoving())
				{
					Enemy->PatrolComponent->MoveToNextPatrolPoint();
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					//UE_LOG(LogTemp, Warning, TEXT("Finished Enemy Patrol task"));
				}
			}
		}
	}
}

EBTNodeResult::Type UBTTN_EnemyPatrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyShootAIController* AIController = Cast<AMyShootAIController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		AEnemy* Enemy = Cast<AEnemy>(AIController->GetPawn());
		if (Enemy)
		{
			UMYEnemyMovingComponent* MovingComponent = Enemy->FindComponentByClass<UMYEnemyMovingComponent>();
			if (MovingComponent)
			{
				MovingComponent->StopMove();
			}
		}
	}
	return EBTNodeResult::Type();
}
