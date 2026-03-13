// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyRestartWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyRestartWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;
protected:
	UFUNCTION()
	void OnRestartButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();
};
