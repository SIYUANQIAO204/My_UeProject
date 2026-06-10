// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyShootAIController.generated.h"

class UBehaviorTree;
class UEnvQuery;
class UBlackBoardData;

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyShootAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyShootAIController();

	void OnPossess(APawn* InPawn) override;

	void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void StartPatrolSpline();

	UFUNCTION()
	void StopPatrolSpline();



protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UEnvQuery> CombatLocationQuery;


	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBlackboardData> BlackboardComponent;

private:
	bool bAttchtToPawn = false;
};
