// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Task/BTS_DetectionPlayer.h"
#include "Mycharacter/MyShootAIController.h"
#include "Mycharacter/Enemy.h"
#include "Mycharacter/SightComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_DetectionPlayer::UBTS_DetectionPlayer()
{
	NodeName = TEXT("DetectionPlayer");
	Interval = 0.2f;
}

void UBTS_DetectionPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AMyShootAIController* MyController = Cast<AMyShootAIController>(OwnerComp.GetAIOwner());
	if (MyController)
	{
		AEnemy* MyPawn = Cast<AEnemy>(MyController->GetCharacter());
		if (MyPawn)
		{
			bool bCanSeePlayer = MyPawn->SightComponent->IsTargetInSight();
			MyController->GetBlackboardComponent()->SetValueAsBool(TEXT("CanSeePlayer"), bCanSeePlayer);
			MyController->GetBlackboardComponent()->SetValueAsBool(TEXT("JustSeenPlayer"), MyPawn->SightComponent->IsTargetJustSeen());
			MyController->GetBlackboardComponent()->SetValueAsBool(TEXT("WasInSight"), MyPawn->SightComponent->WasTargetInSight());
		}
	}
}