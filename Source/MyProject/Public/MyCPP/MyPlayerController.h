// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UMyRestartWidget;
/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMyRestartWidget> RestartWidgetClass;

	void CreateRestartWidget();
	
	void DestroyRestartWidget();

private:

	UPROPERTY()
	TObjectPtr<UMyRestartWidget> RestartWidget;
};
