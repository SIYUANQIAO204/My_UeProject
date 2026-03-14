// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Task/BTD_WhetherDetected.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_WhetherDetected::UBTD_WhetherDetected()
{
	NodeName = TEXT("Whether Detected");
}

bool UBTD_WhetherDetected::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		return BlackboardComp->GetValueAsBool(GetSelectedBlackboardKey());
	}
	return false;
}
