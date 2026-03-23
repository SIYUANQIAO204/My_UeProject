// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UMyHealthWidget;
class UHealthWaringWidget;

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyOwnHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UMyHealthWidget> HealthWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UHealthWaringWidget> HealthWaringWidgetClass;

	UPROPERTY()
	TObjectPtr<UMyHealthWidget> WidgetInstance;

	UPROPERTY()
	TObjectPtr<UHealthWaringWidget> HealthWarningWidgetInstance;

	void CreateHealthWidget();

	void UpdateHealthWidget(float HealthPercent);

	UFUNCTION(BlueprintCallable)
	void ShowHealthWarning();

	UFUNCTION(BlueprintCallable)
	void HideHealthWarning();
};
