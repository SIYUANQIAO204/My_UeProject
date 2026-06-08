// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/PlayerContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Mycharacter/Enemy.h"
#include "Mycharacter/MyShootAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UPlayerContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	
	UObject* OwnerObject = QueryInstance.Owner.Get();
	APawn* Querier = Cast<AEnemy>(OwnerObject);
	if (!Querier)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPlayerContext::ProvideContext: Querier is not a Enemy"));
		return;
	}
	AMyShootAIController* MyController = Cast<AMyShootAIController>(Querier->GetController());
	if (!MyController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPlayerContext::ProvideContext: Controller is not a MyShootAIController"));
		return;
	}
	UBlackboardComponent* BlackboardComp = MyController->GetBlackboardComponent();
	AActor* PlayerActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
	if (!PlayerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPlayerContext::ProvideContext: PlayerActor is not set in Blackboard"));
		return;
	}
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, PlayerActor);
}
