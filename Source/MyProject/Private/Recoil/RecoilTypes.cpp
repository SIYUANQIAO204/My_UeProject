// Fill out your copyright notice in the Description page of Project Settings.


#include "Recoil/RecoilTypes.h"

void FRecoilSpring::ApplyImpluse(FAngularImpulse Impluse)
{
	Velocity += Impluse.Torque;
}

void FRecoilSpring::UpdateOffset(float DeltaTime)
{
	// 计算弹力
	FVector SpringForce = -Stiffness * TargetOffset;
	// 计算阻尼力
	FVector DampingForce = -Damping * Velocity;
	// 计算总加速度
	Acceleration = SpringForce + DampingForce;
	// 更新速度和偏移
	Velocity += Acceleration * DeltaTime;
	TargetOffset += Velocity * DeltaTime;
	//CurrentOffset = FMath::VInterpTo(CurrentOffset, TargetOffset, DeltaTime, OffsetMoveSpeed);
	CurrentOffset = TargetOffset;
}

void FRecoilResolver::ResolveRecoil(const FAngularImpulse& Impulse)
{
	CameraSpring.ApplyImpluse(Impulse);
	return;
	FAngularImpulse WeaponImpulse = Impulse;
	FAngularImpulse CameraImpulse ;
	if (abs(WeaponSpring.CurrentOffset.X) > SoftWeaponPitchLimit)
	{
		float Alpha = WeaponSpring.CurrentOffset.X / MaxWeaponPitchOffset;
		Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);
		float WeaponImpulseRatio = 1.0f - FMath::Sqrt(Alpha);
		WeaponImpulse.Torque.X *= WeaponImpulseRatio;
		CameraImpulse.Torque.X = Impulse.Torque.X * (1.0f - WeaponImpulseRatio);
	}
	if (abs(WeaponSpring.CurrentOffset.Z) > SoftWeaponRollLimit)
	{
		float Alpha = WeaponSpring.CurrentOffset.Z / MaxWeaponRollOffset;
		Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);
		float WeaponImpulseRatio = 1.0f - FMath::Sqrt(Alpha);
		WeaponImpulse.Torque.Z *= WeaponImpulseRatio;
		CameraImpulse.Torque.Z = Impulse.Torque.Z * (1.0f - WeaponImpulseRatio);
	}
	WeaponSpring.ApplyImpluse(WeaponImpulse);
	CameraSpring.ApplyImpluse(CameraImpulse);
}

void FRecoilResolver::Update(float DeltaTime)
{
	WeaponSpring.UpdateOffset(DeltaTime);
	CameraSpring.UpdateOffset(DeltaTime);
}