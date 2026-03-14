// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyShootAIController.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyShootAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyShootAIController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

};
