// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyRestartWidget.h"
#include "Components/Button.h"
#include "MyCPP/MyPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UMyRestartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (RestartButton != nullptr)
	{
		RestartButton->OnClicked.AddDynamic(this, &UMyRestartWidget::OnRestartButtonClicked);
	}
	if (ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddDynamic(this, &UMyRestartWidget::OnExitButtonClicked);
	}
}

void UMyRestartWidget::OnRestartButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Restart Button Clicked"));
	AMyPlayerController* PlayerController  =Cast<AMyPlayerController>(GetOwningPlayer());
	if(PlayerController)
	{
		PlayerController->DestroyRestartWidget();
	}
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}


void UMyRestartWidget::OnExitButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Exit Button Clicked"));
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}