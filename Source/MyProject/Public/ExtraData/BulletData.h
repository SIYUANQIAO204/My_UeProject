// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "enum/TeamTypes.h"
#include "UObject/Object.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BulletData.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class MYPROJECT_API UBulletData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float Radius = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float Mass = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float Resistance = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	TObjectPtr<AActor> BulletOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_Pawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	ETeam Team = ETeam::Neutral;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float LifeTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float InitialSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	TObjectPtr<UStaticMesh> BulletMesh;

};
