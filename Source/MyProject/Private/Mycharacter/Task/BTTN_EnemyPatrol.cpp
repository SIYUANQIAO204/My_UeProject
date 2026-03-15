// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Task/BTTN_EnemyPatrol.h"
#include "Mycharacter/MyShootAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Component/MyEnemyPatrolComponent.h"
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
				FAIMoveRequest MoveRequest(PatrolPoint);
				MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
				MoveRequest.SetUsePathfinding(true);
				MoveRequest.SetCanStrafe(true);
				FPathFollowingRequestResult ReqResult = AIController->MoveTo(MoveRequest);

				if (ReqResult.Code == EPathFollowingRequestResult::Failed)
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to move to Patrol Point"));
					return EBTNodeResult::Failed;

				}
					return EBTNodeResult::InProgress;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Failed to execute Enemy Patrol task"));
	return EBTNodeResult::Failed;
}

void UBTTN_EnemyPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AMyShootAIController* AIController = Cast<AMyShootAIController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		EPathFollowingStatus::Type MoveStatus = AIController->GetMoveStatus();
		
		if (MoveStatus == EPathFollowingStatus::Idle)
		{
			AEnemy* Enemy = Cast<AEnemy>(AIController->GetPawn());
			if(Enemy && Enemy->PatrolComponent)
			{
				Enemy->PatrolComponent->MoveToNextPatrolPoint();
			}
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}