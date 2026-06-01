// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MYEnemyMovingComponent.h"

// Sets default values for this component's properties
UMYEnemyMovingComponent::UMYEnemyMovingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMYEnemyMovingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMYEnemyMovingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMYEnemyMovingComponent::SetTargetLocation(const FVector& NewTargetLocation)
{
	TargetLocation = NewTargetLocation;
}

void UMYEnemyMovingComponent::StopMove()
{

}

void UMYEnemyMovingComponent::TickMove(float DeltaTime)
{
	if (!GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("No Owner"));
		return;
	}
	FVector ToTarget = TargetLocation - Location;
	FVector DesireDirection = ToTarget.GetSafeNormal();
	float Distance = ToTarget.Size();
	FVector DesireVelocit = DesireDirection;
	float DesireSpeed = MaxSpeed;
	if (Distance > SlowRadius)
	{
		DesireSpeed = MaxSpeed;
	}
	else
	{
		DesireSpeed = MaxSpeed * Distance / SlowRadius;
	}
	FVector Steering = DesireVelocity - Velocity;
	Steering = Steering.GetClampedToMaxSize(MaxAcceleration);
	Velocity += Steering * DeltaTime;
	Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);
	FVector NewLocation = Location + Velocity * DeltaTime;
	GetOwner()->SetActorLocation(NewLocation);
}

