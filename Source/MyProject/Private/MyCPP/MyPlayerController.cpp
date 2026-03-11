// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCPP/MyPlayerController.h"

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn's Location: %s"), *InPawn->GetActorLocation().ToString());
		InPawn->SetActorLocation(FVector(0.f, 0.f, 0.f));
	}
}
