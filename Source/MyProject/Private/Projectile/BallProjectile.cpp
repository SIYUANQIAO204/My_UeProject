// Fill out your copyright notice in the Description page of Project Settings.


/*#include "Projectile/BallProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyPlayer.h"

// Sets default values
ABallProjectile::ABallProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereComponent->InitSphereRadius(25.0f);
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionProfileName(TEXT("Ball"));

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Block);

	SphereComponent->SetSimulatePhysics(false);
	SphereComponent->SetNotifyRigidBodyCollision(true);
	SphereComponent->OnComponentHit.AddDynamic(this, &ABallProjectile::OnHit);


	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->InitialSpeed = 2300.f;
	ProjectileMovementComponent->UpdatedComponent = SphereComponent;
	SphereComponent->SetCollisionResponseToChannel(
		ECC_GameTraceChannel2,
		ECR_Ignore
	);
}

// Called when the game starts or when spawned
void ABallProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(3.0f);
}

// Called every frame
void ABallProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABallProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AMyPlayer* MyPlayer = Cast<AMyPlayer>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("Ball hit"));
	if (OtherActor && OtherActor != this && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ball hit: %s"), *OtherActor->GetName());
		Destroy();
	}
}
*/

#include "Projectile/BallProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyPlayer.h"
#include "Component/HealthComponent.h"
// 构造函数
ABallProjectile::ABallProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// 碰撞组件
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComponent->InitSphereRadius(25.f);
	SetRootComponent(SphereComponent);

	// 碰撞设置
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	// 绑定Overlap事件
	SphereComponent->OnComponentBeginOverlap.AddDynamic(
		this,
		&ABallProjectile::OnOverlapBegin
	);

	// Projectile Movement
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ProjectileMovementComponent->InitialSpeed = 2300.f;
	ProjectileMovementComponent->MaxSpeed = 2300.f;

	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComponent->bForceSubStepping = true;
	ProjectileMovementComponent->UpdatedComponent = SphereComponent;
	PrimaryActorTick.bCanEverTick = true;
}

void ABallProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(3.f);
}

void ABallProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABallProjectile::InitBullet(AActor* NewOwner, ETeam NewTeam)
{
	SetOwner(NewOwner);
	SetTeam(NewTeam);
}

void ABallProjectile::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp, Warning, TEXT("Ball Projectile EndPlay: %s"), *GetName());
}

void ABallProjectile::Destroyed()
{
	UE_LOG(LogTemp, Warning, TEXT("Ball Projectile Destroyed: %s"), *GetName());
}

void ABallProjectile::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	

	if (!OtherActor) return;

	if (OverlappedComp->GetCollisionObjectType() == ECC_WorldStatic) {
		UE_LOG(LogTemp, Warning, TEXT("Ball hit the wall!"));
		Destroy();
		return;
	}

	ETeam OtherTeam = ETeam::Neutral;

	if (OtherActor->Implements<UTeamInterface>())
	{
		OtherTeam = ITeamInterface::Execute_GetTeam(OtherActor);
	}

	if (Team == OtherTeam)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ball hit a friendly actor!"));
		return;
	}
	else {
		if (OtherActor->Implements<UHealthInterface>())
		{
			IHealthInterface::Execute_Damage(OtherActor, Damage);
			UE_LOG(LogTemp, Warning, TEXT("Ball hit an actor!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Ball hit an actor without health!"));
			Destroy();
			return;
		}
		Destroy();
	}
}
