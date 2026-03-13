// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHealthWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;

	void UpdateHealthBar(float HealthPercent);
};
