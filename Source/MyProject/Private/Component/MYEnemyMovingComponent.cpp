// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MYEnemyMovingComponent.h"
#include "GameFramework/Actor.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

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
	Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is null in MYEnemyMovingComponent::BeginPlay"));
		return;
	}
	Location = Owner->GetActorLocation();
	// ...
	
}


// Called every frame
void UMYEnemyMovingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsMoving)
	{
		TickMove(DeltaTime);
		TickRotate(DeltaTime);
	}
	// ...
}

void UMYEnemyMovingComponent::SetTargetLocation(const FVector& NewTargetLocation)
{
	
	if (!Owner) return;
	bIsMoving = true;
	TargetLocation = NewTargetLocation;
	UE_LOG(LogTemp, Warning, TEXT("Setting target location: %s"), *NewTargetLocation.ToString());
	UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetOwner()->GetActorLocation(), TargetLocation);
	if (!Path)
	{
		UE_LOG(LogTemp, Error, TEXT("Path nullptr"));
		return;
	}

	UE_LOG(LogTemp, Warning,
		TEXT("Path Valid=%d Num=%d"),
		Path->IsValid(),
		Path->PathPoints.Num());
	PathPoints = Path->PathPoints;
	for (auto Point : PathPoints)
	{
		UE_LOG(LogTemp, Log, TEXT("Path Point: %s"), *Point.ToString());
	}
	if (PathPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No path points found"));
		bIsMoving = false;
		return;
	}
	PathIndex = 0;
}

void UMYEnemyMovingComponent::StopMove()
{
	bIsMoving = false;
	Velocity = FVector::ZeroVector;
}

void UMYEnemyMovingComponent::TickMove(float DeltaTime)
{
	if (!GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("No Owner"));
		return;
	}
	FVector ToTarget = PathPoints[PathIndex] - Location;
	ToTarget.Z = 0.0f;
	FVector DesireDirection = ToTarget.GetSafeNormal();
	float Distance = ToTarget.Size();
	if (Distance < ArriveRadius)
	{
		SwitchPathPoint();
		if (!bIsMoving)
		{
			return;
		}
		ToTarget = PathPoints[PathIndex] - Location;
		ToTarget.Z = 0.0f;
		DesireDirection = (PathPoints[PathIndex] - Location).GetSafeNormal();
		Distance = (PathPoints[PathIndex] - Location).Size();
	}
	FVector DesireVelocity = DesireDirection;
	float DesireSpeed = MaxSpeed;
	if (Distance > SlowRadius)
	{
		DesireSpeed = MaxSpeed;
	}
	else
	{
		DesireSpeed = MaxSpeed * Distance / SlowRadius;
	}
	DesireVelocity = DesireVelocity * DesireSpeed;
	FVector Steering = DesireVelocity - Velocity;
	Steering = Steering.GetClampedToMaxSize(MaxAcceleration);
	Velocity += Steering * DeltaTime;
	Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);
	FVector NewLocation = Location + Velocity * DeltaTime;
	GetOwner()->SetActorLocation(NewLocation);
	Location = NewLocation;
	Distance = (PathPoints[PathIndex] - Location).Size();
}

void UMYEnemyMovingComponent::TickRotate(float DeltaTime)
{
	if (!GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("No Owner"));
		return;
	}
	FVector MoveDir = Velocity.GetSafeNormal();
	FRotator DesiredRotation = MoveDir.Rotation();
	FRotator NewRotation =
		FMath::RInterpTo(
			GetOwner()->GetActorRotation(),
			DesiredRotation,
			DeltaTime,
			MaxRotationSpeed
		);
	GetOwner()->SetActorRotation(NewRotation);
}

void UMYEnemyMovingComponent::SwitchPathPoint()
{
	PathIndex++;
	if (PathIndex >= PathPoints.Num())
	{
		StopMove();
	}
}



