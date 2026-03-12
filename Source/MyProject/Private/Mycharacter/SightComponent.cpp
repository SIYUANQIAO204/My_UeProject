// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/SightComponent.h"
#include "DrawDebugHelpers.h"
// Sets default values for this component's properties
USightComponent::USightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void USightComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool USightComponent::LinTrace()
{
	if (!TargetActor) return false;
	FVector Start = GetComponentLocation();
	FVector End = TargetActor->GetActorLocation();
	FVector ForwardVector = GetOwner()->GetActorForwardVector();
	FVector DirectionToTarget = (End - Start).GetSafeNormal();
	float Distance = FVector::Distance(Start, End);
	if (Distance > SightRange)
	{
		return false;
	}
	float Dot = FVector::DotProduct(ForwardVector, DirectionToTarget);
	float CosHalfAngle = FMath::Cos(FMath::DegreesToRadians(SightAngle * 0.5f));

	if (Dot < CosHalfAngle)
	{
		return false;
	}
	TArray<const AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());
	IgnoreActors.Add(TargetActor);
	bool bCanSee = CanSeeTarget(Start, End, IgnoreActors);
	if (bCanSee)
	{
		FRotator LookAtRotation = (End - Start).Rotation();

		FRotator NewRotation = GetOwner()->GetActorRotation();
		NewRotation.Yaw = LookAtRotation.Yaw;
		GetOwner()->SetActorRotation(NewRotation);
	}
	return bCanSee;
}

bool USightComponent::CanSeeTarget(FVector Start, FVector End, TArray<const AActor*> IgnoreActors) const
{
	if (!TargetActor) return false;
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(IgnoreActors);
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	return !HitResult.bBlockingHit;

}


// Called every frame
void USightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bIsTargetInSight = LinTrace();
	FVector Start = GetComponentLocation();
	FVector Forward = GetComponentRotation().Vector();
	DrawDebugCone(
		GetWorld(),
		Start,
		Forward,
		SightRange,
		FMath::DegreesToRadians(SightAngle / 2),
		FMath::DegreesToRadians(SightAngle / 2),
		20,
		FColor::Green,
		false,
		-1,
		0,
		1
	);
	// ...
}

