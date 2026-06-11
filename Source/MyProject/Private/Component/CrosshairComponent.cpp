// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CrosshairComponent.h"
#include "MyPlayer.h"
#include "Component/MyShootingComponent.h"

// Sets default values for this component's properties
UCrosshairComponent::UCrosshairComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCrosshairComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCrosshairComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AMyPlayer* MyPlayer = Cast<AMyPlayer>(GetOwner());
	if (MyPlayer)
	{
		AimPoint = MyPlayer->FindComponentByClass<UMyShootingComponent>()->GetAimLocation();
	}
	UpdateScreenPosition(DeltaTime);
	// ...
}

bool UCrosshairComponent::ProjectAimPointToScreen()
{
	APlayerController* PlayerController = GetOwner() ? Cast<APlayerController>(GetOwner()->GetInstigatorController()) : nullptr;
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot project aim point to screen: PlayerController is null in CrosshairComponent!"));
		return false;
	}
	bool bProjected = PlayerController->ProjectWorldLocationToScreen(AimPoint, TargetScreenPosition);
	return bProjected;
}

void UCrosshairComponent::UpdateScreenPosition(float DeltaTime)
{
	if (ProjectAimPointToScreen())
	{
		bIsAimVisible = true;
		float Distance = (CurrentScreenPosition - TargetScreenPosition).Length();
		if (Distance < AimDeadZoneRadius)
		{
			TargetScreenPosition = CurrentScreenPosition;

		}
		else
		{
			CurrentScreenPosition = FMath::Vector2DInterpTo(CurrentScreenPosition, TargetScreenPosition, DeltaTime, AimSmoothingSpeed);
		}
	}
	else
	{
		bIsAimVisible = false;
	}
}

