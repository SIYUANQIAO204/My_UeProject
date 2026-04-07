// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "enum/TeamTypes.h"
#include "../Interface/HealthInterface.h"
#include "Interface/TeamInterface.h"
#include "MyPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UHealthComponent;
class UMyShootingComponent;
class UAimComponent;
class ULuaSubsystem;
class UArrowComponent;
struct FInputActionValue;

UCLASS()
class MYPROJECT_API AMyPlayer : public ACharacter, public IHealthInterface, public ITeamInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayer();

	// 通过 IHealthInterface 继承
	void Death_Implementation() override;
	void Damage_Implementation(float DamageAmount) override;
	ETeam GetTeam_Implementation() const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivate))
	TObjectPtr<UHealthComponent> HealthComponent;

	void Move(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

	void OnAimPressed();

	void OnAimReleased();

	void CallLua();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMapping;

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ShootAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditAnywhere, Category = "Aim")
	TObjectPtr<UAimComponent> AimComponent;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	TObjectPtr<UArrowComponent> MuzzleLocation;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	TObjectPtr<UMyShootingComponent> ShootingComponent;
	
	UPROPERTY()
	TObjectPtr<ULuaSubsystem> LuaSubsystem;

	UPROPERTY(EditAnywhere, Category = "Team")
	ETeam Team = ETeam::Player;

};
