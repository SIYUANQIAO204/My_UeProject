// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MYEnemyMovingComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	ACharacter* CurrentOwner = Cast<ACharacter>(GetOwner());
	if (!CurrentOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is not a Characters in MYEnemyMovingComponent::BeginPlay"));
		return;
	}
	CharacterMovementComponent = CurrentOwner->GetCharacterMovement();
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
	if (!GetOwner()) return;
	bIsMoving = true;
	TargetLocation = NewTargetLocation;
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
	PathIndex = 1;
}

void UMYEnemyMovingComponent::StopMove()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Move"));
	bIsMoving = false;
	Velocity = FVector::ZeroVector;
	CharacterMovementComponent->StopMovementImmediately();
}

void UMYEnemyMovingComponent::TickMove(float DeltaTime)
{
	if (!GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("No Owner"));
		return;
	}
	FVector Target = PathPoints[PathIndex];
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	Target.Z = CurrentLocation.Z;
	FVector ToTarget = Target - GetOwner()->GetActorLocation();
	FVector DesireDirection = ToTarget.GetSafeNormal();
	float Distance = FVector::Dist2D(GetOwner()->GetActorLocation(), Target);
	if (Distance < ArriveRadius)
	{
		SwitchPathPoint();
		if (!bIsMoving)
		{
			return;
		}
		Target = PathPoints[PathIndex];
		Target.Z = CurrentLocation.Z;
		ToTarget = Target - GetOwner()->GetActorLocation();
		DesireDirection = ToTarget.GetSafeNormal();
		Distance = FVector::Dist2D(GetOwner()->GetActorLocation(), Target);
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
	CharacterMovementComponent->MaxWalkSpeed = Velocity.Size();
	Cast<ACharacter>(GetOwner())->AddMovementInput(DesireDirection, 1.f);
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

void UMYEnemyMovingComponent::SetTargetRotation(const FRotator& NewTargetRotation)
{
	if (!GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("No Owner"));
		return;
	}
	bIsRotating = true;
	TargetRotation = NewTargetRotation;
}

void UMYEnemyMovingComponent::TickRotateToTargetRotation(float DeltaTime)
{
	FVector CurrentForward = GetOwner()->GetActorForwardVector();
	FRotator CurrentRotation = CurrentForward.Rotation();
	FRotator NewRotation =
		FMath::RInterpTo(
			CurrentRotation,
			TargetRotation,
			DeltaTime,
			MaxRotationSpeed
		);
	GetOwner()->SetActorRotation(NewRotation);
}



