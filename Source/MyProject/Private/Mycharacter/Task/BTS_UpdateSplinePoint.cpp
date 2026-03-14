// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Task/BTS_UpdateSplinePoint.h"
#include "Mycharacter/MyShootAIController.h"
#include "Mycharacter/Enemy.h"
#include "Component/MyEnemyPatrolComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_UpdateSplinePoint::UBTS_UpdateSplinePoint()
{
	NodeName = TEXT("Update Spline Point");

	Interval = 0.1f;
	RandomDeviation = 0.05f;

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_UpdateSplinePoint, BlackboardKey));
}

void UBTS_UpdateSplinePoint::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AMyShootAIController* AIController = Cast<AMyShootAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
		return;
	AEnemy* Enemy = Cast<AEnemy>(AIController->GetPawn());
	if (Enemy == nullptr)
		return;
	if (!Enemy->PatrolComponent)
		return;
	FVector SplinePointLocation = Enemy->PatrolComponent->GetNextPatrolPoint();
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), SplinePointLocation);
}
