// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "HUD/MyHealthWidget.h"
#include "HUD/HealthWaringWidget.h"
#include "HUD/AimWidget.h"
#include "HUD/InnerCrooshair.h"

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

void AMyOwnHUD::ShowAimWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (AimWidgetClass != nullptr)
	{
		if (AimWidgetInstance == nullptr)
		{
			AimWidgetInstance = CreateWidget<UAimWidget>(PlayerController, AimWidgetClass);
			if (AimWidgetInstance != nullptr)
			{
				AimWidgetInstance->AddToViewport();
			}
		}
		else
		{
			AimWidgetInstance->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		
	}
}

void AMyOwnHUD::ShowCrosshair()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (CrosshairClass != nullptr)
	{
		if (CrosshairInstance == nullptr)
		{
			CrosshairInstance = CreateWidget<UInnerCrooshair>(PlayerController, CrosshairClass);
			if (CrosshairInstance != nullptr)
			{
				CrosshairInstance->AddToViewport();
			}
		}
		else
		{
			CrosshairInstance->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

	}
}

void AMyOwnHUD::HideCrosshair()
{
	if (CrosshairInstance != nullptr)
	{
		CrosshairInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMyOwnHUD::UpdateCrosshair(FVector2D ScreenPosition)
{
	if (CrosshairInstance != nullptr)
	{
		CrosshairInstance->SetPositionInViewport(ScreenPosition);
	}
}

void AMyOwnHUD::HideAimWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();	
	if (AimWidgetClass != nullptr && AimWidgetInstance != nullptr)
	{
		AimWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
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

