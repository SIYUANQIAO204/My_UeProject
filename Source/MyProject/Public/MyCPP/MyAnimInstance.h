// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

class AMyPlayer;
class UCharacterMovementComponent;


UCLASS()
class MYPROJECT_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	public:
		//UMyAnimInstance();
		virtual void NativeInitializeAnimation() override;
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;
		
		UPROPERTY(BlueprintReadOnly)
		TObjectPtr<AMyPlayer> PlayerCharacter;

		UPROPERTY(BlueprintReadOnly)
		TObjectPtr<UCharacterMovementComponent> PlayerCharacterMovement;

		UPROPERTY(BlueprintReadOnly)
		float Speed;
};
