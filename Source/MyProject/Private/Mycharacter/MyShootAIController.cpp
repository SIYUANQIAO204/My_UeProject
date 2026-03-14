// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/MyShootAIController.h"
#include "BehaviorTree/BehaviorTree.h"

AMyShootAIController::AMyShootAIController()
{
}

void AMyShootAIController::BeginPlay()
{
	Super::BeginPlay();
	if(BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}
