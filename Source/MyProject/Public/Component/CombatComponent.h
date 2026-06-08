// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Mycharacter/CombatTask/CombatState.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class UEnvQuery;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitCombat(TObjectPtr<AActor> InTarget);

	UFUNCTION()
	void ExitCombat();

	UFUNCTION()
	void EnterFindPosition();

	UFUNCTION()
	void ExitFindPosition();

	UFUNCTION()
	void TickFindPosition();

	UFUNCTION()
	void EnterMoving();

	UFUNCTION()
	void ExitMoving();

	UFUNCTION()
	void TickMoving();

	UFUNCTION()
	void EnterShooting();

	UFUNCTION()
	void ExitShooting();

	UFUNCTION()
	void TickShooting();

	UFUNCTION()
	void ChangeState(ECombatState NewState);

	UFUNCTION()
	void EnterNewState(ECombatState NewState);

	UFUNCTION()
	void ExitCurrentState();

	UFUNCTION()
	void RunCombatPositionQuery();

	void CheckCurrentPosition();

	void OnCombatPositionQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	float ScoreCurrentLocation() const;

	FORCEINLINE bool IsInCombat() const { return bInCombat; }

	FORCEINLINE ECombatState GetCurrentState() const { return CurrentState; }

protected:
	UPROPERTY(VisibleAnywhere)
	bool bInCombat = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AController> OwnerController;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> Target;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UEnvQuery> CombatPositionQuery;

	UPROPERTY(VisibleAnywhere)
	ECombatState CurrentState = ECombatState::Finished;

	UPROPERTY(VisibleAnywhere)
	FVector CombatTargetLocation;

	FTimerHandle CombatPositionQueryHandler;

	UPROPERTY(EditAnywhere, Category = "CombatDistance")
	float InnerRadius = 400.f;

	UPROPERTY(EditAnywhere, Category = "CombatDistance")
	float OuterRadius = 800.f;

	UPROPERTY(EditAnywhere, Category = "CombatDistance")
	float DesiredDistance = 600.f;

	UPROPERTY(EditAnywhere, Category = "CombatDistance")
	float DistanceTolerance = 100.f;

	UPROPERTY(EditAnywhere, Category = "CombatDistance")
	float MinDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = "CombatDistance")
	float MaxDistance = 1000.f;

	UPROPERTY(EditAnywhere, Category = "CombatDistance")
	int NumberOfRings = 4;

	UPROPERTY(EditAnywhere, Category = "CombatDistance")
	int NumberOfPoints = 8;

	UPROPERTY(EditAnywhere, Category = "CombatDistance")
	float AngleOffset = 180.f;

};
