// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHealthWidget.h"
#include "Components/ProgressBar.h"

void UMyHealthWidget::UpdateHealthBar(float HealthPercent)
{
	HealthProgressBar->SetPercent(HealthPercent);
}
