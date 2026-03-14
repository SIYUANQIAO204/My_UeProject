// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Task/BTTaskNode_MyShoot.h"
#include "Mycharacter/MyShootAIController.h"
#include "GameFramework/Character.h"
#include "Mycharacter/Enemy.h"
#include "Component/MyShootingComponent.h"

UBTTaskNode_MyShoot::UBTTaskNode_MyShoot()
{
	NodeName = TEXT("MyShoot");
	bNotifyTick = true;
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
			return;
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return;
}


EBTNodeResult::Type UBTTaskNode_MyShoot::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyShootAIController* MyController = Cast<AMyShootAIController>(OwnerComp.GetAIOwner());
	if (MyController)
	{
		AEnemy* MyPawn = Cast<AEnemy>(MyController->GetCharacter());
		if (MyPawn)
		{
			MyPawn->ShootingComponent->StopShooting();
		}
	}
	return EBTNodeResult::Aborted;
}