// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SightComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API USightComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USightComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool LinTrace();

	bool CanSeeTarget(FVector Start, FVector End, TArray<const AActor*> IgnoreActors) const;

	TObjectPtr<AActor> TargetActor;
	bool bIsTargetInSight = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetTargetActor(AActor* NewTarget) { TargetActor = NewTarget; }

	FORCEINLINE bool IsTargetInSight() const { return bIsTargetInSight; }

		
};
