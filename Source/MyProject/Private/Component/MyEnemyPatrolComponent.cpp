// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MyEnemyPatrolComponent.h"
#include "Components/SplineComponent.h"

// Sets default values for this component's properties
UMyEnemyPatrolComponent::UMyEnemyPatrolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyEnemyPatrolComponent::BeginPlay()
{
	Super::BeginPlay();
	if (RouteActor)
	{
		SplineComponent = RouteActor->FindComponentByClass<USplineComponent>();
		if (SplineComponent)
		{
			float ClosestDistance = SplineComponent->FindInputKeyClosestToWorldLocation(GetOwner()->GetActorLocation());

			CurrentRouteIndex = FMath::RoundToInt(ClosestDistance);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RouteActor is not set for %s"), *GetOwner()->GetName());
	}
	// ...
	
}

FVector UMyEnemyPatrolComponent::GetNextPatrolPoint()
{
	if(!RouteActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("RouteActor is not set for %s"), *GetOwner()->GetName());
		return FVector::ZeroVector;
	}

	if (SplineComponent)
	{
		FVector GoalLocation = SplineComponent->GetLocationAtSplinePoint(CurrentRouteIndex, ESplineCoordinateSpace::World);
		CurrentRouteIndex = (CurrentRouteIndex + 1) % SplineComponent->GetNumberOfSplinePoints();
		return GoalLocation;
	}
	return FVector::ZeroVector;
}

// Called every frame
void UMyEnemyPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

