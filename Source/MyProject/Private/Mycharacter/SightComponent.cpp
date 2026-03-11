// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/SightComponent.h"

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
	TArray<const AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());
	IgnoreActors.Add(TargetActor);
	bIsTargetInSight = CanSeeTarget(Start, End, IgnoreActors);
	if (bIsTargetInSight)
	{
		FRotator LookAtRotation = (End - Start).Rotation();
		GetOwner()->SetActorRotation(LookAtRotation);
	}
	return bIsTargetInSight;
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
	// ...
}

