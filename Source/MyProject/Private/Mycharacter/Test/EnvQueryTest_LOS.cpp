// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter/Test/EnvQueryTest_LOS.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "MyPlayer.h"

UEnvQueryTest_LOS::UEnvQueryTest_LOS()
{
	Cost = EEnvTestCost::High;
	ValidItemType =
		UEnvQueryItemType_Point::StaticClass();
	TraceChannel = ECC_Visibility;
}

void UEnvQueryTest_LOS::RunTest(FEnvQueryInstance& QueryInstance) const
{
	TArray<FVector> ContextLocations;
	QueryInstance.PrepareContext(TargetContext, ContextLocations);
	FVector TargetLocation = ContextLocations[0];
	UWorld* World = QueryInstance.World;
	AActor* QuerierActor = Cast<AActor>(QueryInstance.Owner.Get());
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(QuerierActor);
	FHitResult HitResult;
	bool bHit = false;
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex())+FVector(0, 0, 80);
		bHit = World->LineTraceSingleByChannel(HitResult, ItemLocation, TargetLocation, TraceChannel, TraceParams);
		AMyPlayer* HitActor = Cast<AMyPlayer>(HitResult.GetActor());
		float Score = 0.5f;
		if(!bHit || !HitActor)
		{
			Score = 0.0f;
		}
		It.SetScore(TestPurpose, FilterType, Score, 0.f, 1.f);
	}
}
