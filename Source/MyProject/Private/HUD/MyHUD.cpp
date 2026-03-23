// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "HUD/MyHealthWidget.h"
#include "HUD/HealthWaringWidget.h"

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

void AMyOwnHUD::ShowHealthWarning()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (HealthWaringWidgetClass != nullptr)
	{
		if (HealthWarningWidgetInstance == nullptr)
		{
			HealthWarningWidgetInstance = CreateWidget<UHealthWaringWidget>(PlayerController, HealthWaringWidgetClass);
			if (HealthWarningWidgetInstance != nullptr)
			{
				HealthWarningWidgetInstance->AddToViewport();
			}
		}
		else
		{
			HealthWarningWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		}
		
	}
}

void AMyOwnHUD::HideHealthWarning()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (HealthWaringWidgetClass != nullptr && HealthWarningWidgetInstance != nullptr)
	{
		HealthWarningWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}
