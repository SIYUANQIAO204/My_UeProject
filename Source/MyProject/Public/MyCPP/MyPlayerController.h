// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UMyRestartWidget;
class UMyHealthWidget;
class AMyOwnHUD;
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMyHealthWidget> HealthWidgetClass;

	void UpdateHealthWidget(float HealthPercent);

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TObjectPtr<UMyRestartWidget> RestartWidget;

	UPROPERTY()
	TObjectPtr<UMyHealthWidget> HealthWidget;

	UPROPERTY()
	TObjectPtr<AMyOwnHUD> MyOwnHUD;
};
