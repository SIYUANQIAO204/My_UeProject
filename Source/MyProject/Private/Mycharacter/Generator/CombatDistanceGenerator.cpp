// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Generator/CombatDistanceGenerator.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "NavigationSystem.h"

UCombatDistanceGenerator::UCombatDistanceGenerator()
{
	ItemType = UEnvQueryItemType_Actor::StaticClass();
}

void UCombatDistanceGenerator::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	InnerRadius.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);
	OuterRadius.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);
	NumberOfRings.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);
	NumberOfPoints.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);
	AngleOffset.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);

	float InnerRadiusValue = InnerRadius.GetValue();
	float OuterRadiusValue = OuterRadius.GetValue();
	int32 NumberOfRingsValue = NumberOfRings.GetValue();
	int32 NumberOfPointsValue = NumberOfPoints.GetValue();
	float AngleOffsetValue = AngleOffset.GetValue();

	float RadiusStep = (OuterRadiusValue - InnerRadiusValue) / NumberOfRingsValue;
	float StartAngle = -AngleOffsetValue * 0.5f;
	float EndAngle = AngleOffsetValue * 0.5f;
	TArray<FVector> ContextLocations;
	FVector PlayerLocation = ContextLocations.Num() > 0 ? ContextLocations[0] : FVector::ZeroVector;
	TArray<FNavLocation> GeneratedLocations;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(QueryInstance.World);
	for (float Radius = InnerRadiusValue; Radius <= OuterRadiusValue; Radius += RadiusStep)
	{
		for (int32 i = 0; i < NumberOfPointsValue; i++)
		{
			float Angle = ((EndAngle - StartAngle) / NumberOfPointsValue) * i + StartAngle;
			float Radians = FMath::DegreesToRadians(Angle);
			FVector PointLocation = PlayerLocation + FVector(FMath::Cos(Radians), FMath::Sin(Radians), 0) * Radius;
			FNavLocation NavLocation;
			if (NavSys->ProjectPointToNavigation(PointLocation, NavLocation, FVector(100.f, 100.f, 500.f)))
			{
				PointLocation = NavLocation.Location;
				GeneratedLocations.Add(NavLocation);
			}
		}
	}
	
	ProjectAndFilterNavPoints(GeneratedLocations, QueryInstance);
	StoreNavPoints(GeneratedLocations, QueryInstance);
}
