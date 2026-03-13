// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCPP/MyPlayerController.h"
#include "HUD/MyRestartWidget.h"
#include "HUD/MyHealthWidget.h"

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn's Location: %s"), *InPawn->GetActorLocation().ToString());
	}
}

void AMyPlayerController::CreateRestartWidget()
{
	if (RestartWidgetClass != nullptr)
	{
		SetPause(true);
		SetInputMode(FInputModeUIOnly());

		bShowMouseCursor = true;
		RestartWidget = CreateWidget<UMyRestartWidget>(this, RestartWidgetClass);
		RestartWidget->AddToViewport();

	}
	
}

void AMyPlayerController::DestroyRestartWidget()
{
	RestartWidget->RemoveFromParent();
	RestartWidget->Destruct();
	SetPause(false);
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void AMyPlayerController::UpdateHealthWidget(float HealthPercent)
{
	if (HealthWidgetClass != nullptr)
	{
		HealthWidget->UpdateHealthBar(HealthPercent);
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (HealthWidgetClass != nullptr)
	{
		HealthWidget = CreateWidget<UMyHealthWidget>(this, HealthWidgetClass);
		HealthWidget->AddToViewport();
		
	}

}
