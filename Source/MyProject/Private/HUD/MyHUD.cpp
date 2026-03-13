// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "HUD/MyHealthWidget.h"

void AMyOwnHUD::CreateHealthWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (HealthWidgetClass != nullptr)
	{
		WidgetInstance = CreateWidget<UMyHealthWidget>(PlayerController, HealthWidgetClass);
		if (WidgetInstance != nullptr)
		{
			WidgetInstance->AddToViewport();
		}
	}
}

void AMyOwnHUD::UpdateHealthWidget(float HealthPercent)
{
	if (WidgetInstance != nullptr)
	{
		WidgetInstance->UpdateHealthBar(HealthPercent);
	}
}
