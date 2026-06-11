// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UMyHealthWidget;
class UHealthWaringWidget;
class UAimWidget;
class UInnerCrooshair;


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

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UAimWidget> AimWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UInnerCrooshair> CrosshairClass;

	UPROPERTY()
	TObjectPtr<UMyHealthWidget> WidgetInstance;

	UPROPERTY()
	TObjectPtr<UAimWidget> AimWidgetInstance;

	UPROPERTY()
	TObjectPtr<UHealthWaringWidget> HealthWarningWidgetInstance;

	UPROPERTY()
	TObjectPtr<UInnerCrooshair> CrosshairInstance;

	void CreateHealthWidget();

	void UpdateHealthWidget(float HealthPercent);

	void ShowAimWidget();

	void ShowCrosshair();

	void HideCrosshair();

	void UpdateCrosshair(FVector2D ScreenPosition);

	void HideAimWidget();

	UFUNCTION(BlueprintCallable)
	void ShowHealthWarning();

	UFUNCTION(BlueprintCallable)
	void HideHealthWarning();
};
