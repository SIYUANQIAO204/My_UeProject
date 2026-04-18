
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enum/TeamTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;


UCLASS()
class MYPROJECT_API ABallProjectile : public AActor
{
	GENERATED_BODY()

public:
	ABallProjectile();

protected:
	UPROPERTY(EditAnywhere, Category = "Damage",meta = (AllowPrivateAccess = "true"))
	float Damage = 20.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
	ETeam Team = ETeam::Neutral;

	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball",meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Ball")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	

protected:

	virtual void BeginPlay() override;

	/** 子弹检测玩家 */
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void SetOwner(AActor* NewOwner){ OwnerActor = NewOwner;}

	FORCEINLINE void SetTeam(ETeam NewTeam) { Team = NewTeam; }

	UFUNCTION()
	void InitBullet(AActor* NewOwner, ETeam NewTeam);

	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void Destroyed() override;

	void InitVelocity(FVector Direction);

};