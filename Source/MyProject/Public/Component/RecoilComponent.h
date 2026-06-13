// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Recoil/RecoilTypes.h"
#include "RecoilComponent.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API URecoilComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URecoilComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FRecoilResolver RecoilResolver;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ApplyRecoil(const FAngularImpulse& Impulse);

	
	FORCEINLINE FVector GetWeaponRecoilOffset() const
	{
		return RecoilResolver.WeaponSpring.CurrentOffset;
	}

	FORCEINLINE FVector GetCameraRecoilOffset() const
	{
		return RecoilResolver.CameraSpring.CurrentOffset;
	}
		
};
