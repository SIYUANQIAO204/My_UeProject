// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrosshairComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UCrosshairComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCrosshairComponent();

	void UpdateCrosshair(float DeltaTime);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	FVector AimPoint;

	UPROPERTY(BlueprintReadOnly)
	FVector2D TargetScreenPosition;

	UPROPERTY(BlueprintReadOnly)
	FVector2D CurrentScreenPosition;

	UPROPERTY(BlueprintReadOnly)
	FVector2D ScreenCenter;

	bool bIsAimVisible;

	UPROPERTY(EditAnywhere)
	float AimSmoothingSpeed = 10.0f;

	UPROPERTY(EditAnywhere)
	float AimVisibilityDistance = 500.0f;

	UPROPERTY(EditAnywhere)
	float AimDeadZoneRadius = 50.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
		
};
