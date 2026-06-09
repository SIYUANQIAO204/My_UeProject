// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CombatComponent.h"
#include "MyCharacter/Enemy.h"
#include "Component/MyShootingComponent.h"
#include "Component/MYEnemyMovingComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "MyPlayer.h"
#include "EnvironmentQuery/EnvQueryManager.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	OwnerController = Cast<AController>(OwnerCharacter->GetController());
	// ...
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	switch (CurrentState)
	{
	case ECombatState::Moving:
		TickMoving(DeltaTime);
		break;
	case ECombatState::Aiming:
		TickAiming(DeltaTime);
		break;
	default:
		break;
	}
	// ...
}

void UCombatComponent::InitCombat(TObjectPtr<AActor> InTarget)
{
	Target = InTarget;
	SetComponentTickEnabled(true);
	bInCombat = true;
	ChangeState(ECombatState::FindPosition);
}

void UCombatComponent::ExitCombat()
{
	SetComponentTickEnabled(false);
	CurrentState = ECombatState::Finished;
	bInCombat = false;
}

void UCombatComponent::EnterFindPosition()
{
	RunCombatPositionQuery();
}


void UCombatComponent::EnterMoving()
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerCharacter);
	if (Enemy)
	{
		Enemy->EnemyMovingComponent->StopMove();
		Enemy->EnemyMovingComponent->SetTargetLocation(CombatTargetLocation);
	}
}

void UCombatComponent::ExitMoving()
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerCharacter);
	if (Enemy)
	{
		Enemy->EnemyMovingComponent->StopMove();
	}
}

void UCombatComponent::TickMoving(float DeltaTime)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerCharacter);
	if (Enemy)
	{
		if (!Enemy->EnemyMovingComponent->IsMoving())
		{
			ChangeState(ECombatState::Aiming);
		}
	}
}

void UCombatComponent::EnterShooting()
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerCharacter);
	if (Enemy)
	{
		Enemy->ShootingComponent->StartShooting();
		GetWorld()->GetTimerManager().SetTimer(CombatPositionQueryHandler, this, &UCombatComponent::CheckCurrentPosition, 5.f, true);
	}
}

void UCombatComponent::ExitShooting()
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerCharacter);
	if (Enemy)
	{
		Enemy->ShootingComponent->StopShooting();
		GetWorld()->GetTimerManager().ClearTimer(CombatPositionQueryHandler);
	}
}


void UCombatComponent::ChangeState(ECombatState NewState)
{
	ExitCurrentState();
	CurrentState = NewState;
	EnterNewState(NewState);
}

void UCombatComponent::EnterNewState(ECombatState NewState)
{
	switch (NewState)
	{
	case ECombatState::FindPosition:
		EnterFindPosition();
		break;
	case ECombatState::Moving:
		EnterMoving();
		break;
	case ECombatState::Shooting:
		EnterShooting();
		break;
	case ECombatState::Aiming:
		EnterAiming();
		break;
	case ECombatState::Finished:
		ExitCombat();
		break;
	default:
		break;
	}
}

void UCombatComponent::ExitCurrentState()
{
	switch (CurrentState)
	{
	case ECombatState::Moving:
		ExitMoving();
		break;
	case ECombatState::Shooting:
		ExitShooting();
		break;
	default:
		break;
	}
}

void UCombatComponent::RunCombatPositionQuery()
{
	if (!CombatPositionQuery || !OwnerCharacter) return;

	FEnvQueryRequest Request(
		CombatPositionQuery,
		OwnerCharacter
	);
	Request.SetFloatParam(
		TEXT("OuterRadius"),
		OuterRadius
	);
	Request.SetFloatParam(
		TEXT("InnerRadius"),
		InnerRadius
	);
	Request.SetFloatParam(
		TEXT("DesiredDistance"),
		DesiredDistance
	);
	Request.SetFloatParam(
		TEXT("DistanceTolerance"),
		DistanceTolerance
	);
	Request.SetIntParam(
		TEXT("NumberOfRings"),
		NumberOfRings
	);
	Request.SetIntParam(
		TEXT("NumberOfPoints"),
		NumberOfPoints
	);
	Request.SetFloatParam(
		TEXT("AngleOffset"),
	AngleOffset
	);
	Request.SetFloatParam(
		TEXT("MinDistance"),
		MinDistance
	);
	Request.SetFloatParam(
		TEXT("MaxDistance"),
		MaxDistance
	);

	Request.Execute(
		EEnvQueryRunMode::SingleResult,
		this,
		&UCombatComponent::OnCombatPositionQueryFinished
	);
}

void UCombatComponent::EnterAiming()
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerCharacter);
	if (Enemy)
	{
		Enemy->ShootingComponent->SetAimLocation(Target->GetActorLocation());
	}
}



void UCombatComponent::TickAiming(float DeltaTime)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerCharacter);
	if (Enemy)
	{
		Enemy->ShootingComponent->UpdateAim(DeltaTime);
		if (Enemy->ShootingComponent->IsAimingFinished())
		{
			ChangeState(ECombatState::Shooting);
		}
	}
}

void UCombatComponent::CheckCurrentPosition()
{
	float CurrentScore = ScoreCurrentLocation();
	UE_LOG(LogTemp, Warning, TEXT("Current Score: %f"), CurrentScore);
	if (CurrentScore < 0.7f)
	{
		ChangeState(ECombatState::FindPosition);
	}
}



void UCombatComponent::OnCombatPositionQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (!Result->IsSuccessful())
	{
		ChangeState(ECombatState::Finished);
		return;
	}
	CombatTargetLocation = Result->GetItemAsLocation(0);
	ChangeState(ECombatState::Moving);
}

float UCombatComponent::ScoreCurrentLocation() const
{
	FVector TargetLocation = Target->GetActorLocation();
	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	float DistanceToTarget = FVector::Dist(CurrentLocation, TargetLocation);
	float Score = 0.f;
	Score += FMath::Exp(-FMath::Square(DistanceToTarget - DesiredDistance) / (2.f * FMath::Square(DistanceTolerance)));
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(OwnerCharacter);
	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CurrentLocation, TargetLocation, ECC_Visibility, TraceParams);
	
	AMyPlayer* HitActor = Cast<AMyPlayer>(HitResult.GetActor());
	if (!bHit || !HitActor)
	{
		Score *= 0.1f; // Reduce score if there's an obstacle
	}
	return Score;
}
