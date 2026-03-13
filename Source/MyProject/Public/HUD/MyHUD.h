// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UMyHealthWidget;

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

	UPROPERTY()
	TObjectPtr<UMyHealthWidget> WidgetInstance;

	void CreateHealthWidget();

	void UpdateHealthWidget(float HealthPercent);
};
