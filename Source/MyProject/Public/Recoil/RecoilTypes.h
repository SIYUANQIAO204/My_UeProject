// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecoilTypes.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FAngularImpulse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FVector Torque = FVector::ZeroVector;

	FORCEINLINE FAngularImpulse ScalingImpulse(float Scale) const
	{
		FAngularImpulse ScaledImpulse;
		ScaledImpulse.Torque = Torque * Scale;
		return ScaledImpulse;
	}

};

USTRUCT(BlueprintType)
struct FRecoilSpring
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float Stiffness = 100.0f; //弹力系数

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float Damping = 20.0f; //阻尼系数

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float OffsetMoveSpeed = 5.0f;


	UPROPERTY(VisibleAnywhere)
	FVector Velocity = FVector::ZeroVector; //当前速度

	UPROPERTY(VisibleAnywhere)
	FVector Acceleration = FVector::ZeroVector; //当前加速度

	UPROPERTY(VisibleAnywhere)
	FVector CurrentOffset = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere)
	FVector TargetOffset = FVector::ZeroVector;


	void ApplyImpluse(FAngularImpulse Impluse);

	void UpdateOffset(float DeltaTime);

};

USTRUCT(BlueprintType)
struct FRecoilResolver
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FRecoilSpring WeaponSpring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FRecoilSpring CameraSpring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch")
	float MaxWeaponPitchOffset = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch")
	float SoftWeaponPitchLimit = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roll")
	float SoftWeaponRollLimit = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roll")
	float MaxWeaponRollOffset = 15.0f;


	void ResolveRecoil(const FAngularImpulse& Impulse);

	void Update(float DeltaTime);
};
