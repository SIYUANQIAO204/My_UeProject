// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/MyShootAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Component/MyEnemyPatrolComponent.h"
#include "Component/MyShootingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Component/MYEnemyMovingComponent.h"

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

void AMyShootAIController::StartPatrolSpline()
{
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		UMyEnemyPatrolComponent* PatrolComponent = ControlledPawn->FindComponentByClass<UMyEnemyPatrolComponent>();
		UMYEnemyMovingComponent* MovingComponent = ControlledPawn->FindComponentByClass<UMYEnemyMovingComponent>();
		if (PatrolComponent && MovingComponent)
		{
			FVector NextPatrolPoint = PatrolComponent->GetNextPatrolPoint();
			MovingComponent->SetTargetLocation(NextPatrolPoint);
		}
	}
}

void AMyShootAIController::StopPatrolSpline()
{
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		UMyEnemyPatrolComponent* PatrolComponent = ControlledPawn->FindComponentByClass<UMyEnemyPatrolComponent>();
		UMYEnemyMovingComponent* MovingComponent = ControlledPawn->FindComponentByClass<UMYEnemyMovingComponent>();
		if (PatrolComponent && MovingComponent)
		{
			MovingComponent->StopMove();
		}
	}
}

