// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Test/EnvQueryTest_CombatDistance.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

UEnvQueryTest_CombatDistance::UEnvQueryTest_CombatDistance()
{
	Cost = EEnvTestCost::Low;
	ValidItemType =
		UEnvQueryItemType_Point::StaticClass();
}

void UEnvQueryTest_CombatDistance::RunTest(FEnvQueryInstance& QueryInstance) const
{
	DesiredDistance.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);
	DistanceTolerance.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);
	MinDistance.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);
	MaxDistance.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);

	float DesiredDistanceValue = DesiredDistance.GetValue();
	float DistanceToleranceValue = DistanceTolerance.GetValue();
	float MinDistanceValue = MinDistance.GetValue();
	float MaxDistanceValue = MaxDistance.GetValue();

	TArray<FVector> ContextLocations;
	QueryInstance.PrepareContext(TargetContext, ContextLocations);
	FVector TargetLocation = ContextLocations[0];
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
		float DistanceToTarget = FVector::Dist(ItemLocation, TargetLocation);
		if (DistanceToTarget < MinDistanceValue || DistanceToTarget > MaxDistanceValue)
		{
			It.ForceItemState(EEnvItemStatus::Failed);
			continue;
		}
		float Error = DistanceToTarget - DesiredDistanceValue;
		float Score = FMath::Exp(-Error * Error / (2.f * DistanceToleranceValue * DistanceToleranceValue));
		It.SetScore(TestPurpose, FilterType, Score, 0.f, 1.f);
	}
}
