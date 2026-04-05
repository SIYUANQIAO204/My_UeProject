// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/HealthInterface.h"

// Add default functionality here for any IHealthInterface functions that are not pure virtual.

void IHealthInterface::Death_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Default Death_Implementation"));
}

void IHealthInterface::Damage_Implementation(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Default Damage_Implementation"))
}
