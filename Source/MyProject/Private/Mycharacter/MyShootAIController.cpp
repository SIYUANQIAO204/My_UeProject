// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/MyShootAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"


AMyShootAIController::AMyShootAIController()
{
	bWantsPlayerState = true;
	bSetControlRotationFromPawnOrientation = true;
}

void AMyShootAIController::BeginPlay()
{
	Super::BeginPlay();
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
		UE_LOG(LogTemp, Warning, TEXT("Behavior Tree started successfully"));
		if (GetBlackboardComponent())
		{
			UE_LOG(LogTemp, Log, TEXT("Blackboard initialized!"));
		}
	}
}

void AMyShootAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetControlRotation(InPawn->GetActorRotation());
	bAttchtToPawn = true;
	
}

void AMyShootAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//UE_LOG(LogTemp, Warning, TEXT("Ticking AI Controller"));
}