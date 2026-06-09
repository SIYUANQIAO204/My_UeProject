// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Task/BTTaskNode_MyShoot.h"
#include "Mycharacter/MyShootAIController.h"
#include "GameFramework/Character.h"
#include "Mycharacter/Enemy.h"
#include "Component/CombatComponent.h"
#include "Component/MyShootingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_MyShoot::UBTTaskNode_MyShoot()
{
	NodeName = TEXT("MyShoot");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_MyShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Execute Shoot Task"));
	AMyShootAIController* MyController = Cast<AMyShootAIController>(OwnerComp.GetAIOwner());
	if (MyController)
	{
		AEnemy* MyPawn = Cast<AEnemy>(MyController->GetCharacter());
		if (MyPawn)
		{
			//MyPawn->ShootingComponent->StartShooting();
			TObjectPtr<AActor> Target = Cast<AActor>(MyController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
			MyPawn->CombatComponent->InitCombat(Target);
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
			if(!MyPawn->CombatComponent->IsInCombat())
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return;
			}
		}
	}
}


EBTNodeResult::Type UBTTaskNode_MyShoot::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyShootAIController* MyController = Cast<AMyShootAIController>(OwnerComp.GetAIOwner());
	if (MyController)
	{
		AEnemy* MyPawn = Cast<AEnemy>(MyController->GetCharacter());
		if (MyPawn)
		{
			//MyPawn->ShootingComponent->StopShooting();
			MyPawn->CombatComponent->ExitCombat();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Abort Shoot Task"));
	return EBTNodeResult::Aborted;
}