// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCameraManageComponent.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UPlayerCameraManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerCameraManageComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitCamera();

	FORCEINLINE void SetCameraComponent(UCameraComponent* InCameraComponent) { CameraComponent = InCameraComponent; }

	FORCEINLINE void SetSpringArmComponent(USpringArmComponent* InSpringArmComponent) { SpringArmComponent = InSpringArmComponent; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmLength", meta = (AllowPrivateAccess))
	float DefaultArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmLength", meta = (AllowPrivateAccess))
	float AimArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmLength", meta = (AllowPrivateAccess))
	FRotator DefaultCameraRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmLength", meta = (AllowPrivateAccess))
	FRotator AimCameraRotation;

	UPROPERTY()
	FVector AimDirection;

	UPROPERTY(EditAnywhere, Category = "Aim", meta = (AllowPrivateAccess = "true"))
	FVector AimOffset = FVector(0.0f, 50.0f, 30.0f);

	

		
};
