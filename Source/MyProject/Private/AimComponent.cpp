// Fill out your copyright notice in the Description page of Project Settings.


#include "AimComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAimComponent::UAimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false; // 默认不启用 Tick，只有在瞄准时才启用

	// ...
}


// Called when the game starts
void UAimComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAimComponent::GetAimDirection(FVector MuzzleLocation, FVector& AimDirection)
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return false;

    // 1. 获取屏幕中心
    int32 SizeX, SizeY;
    PlayerController->GetViewportSize(SizeX, SizeY);
    FVector2D CrosshairPos(SizeX * 0.5f, SizeY * 0.5f);

    // 2. 反投影
    FVector WorldLocation;
    FVector WorldDirection;
    if (!UGameplayStatics::DeprojectScreenToWorld(
        PlayerController,
        CrosshairPos,
        WorldLocation,
        WorldDirection))
    {
        return false;
    }

    // 3. Trace
    FVector Start = WorldLocation;
    FVector End = Start + WorldDirection * 10000.f;

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility
    );

    // 4. 目标点
    FVector TargetPoint;
    if (HitResult.bBlockingHit)
    {
        TargetPoint = HitResult.ImpactPoint;
    }
    else
    {
        TargetPoint = End;
    }

    // 5. 计算方向（核心）
    AimDirection = (TargetPoint - MuzzleLocation).GetSafeNormal();

    return true;
}

void UAimComponent::SetIsAiming(bool bNewIsAiming)
{
	bIsAiming = bNewIsAiming;
    if (bIsAiming)
    {
		SetComponentTickEnabled(true); // 启用 Tick
    }
    else
	{
		SetComponentTickEnabled(false); // 禁用 Tick
    }
}

