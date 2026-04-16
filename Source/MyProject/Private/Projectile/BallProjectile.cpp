
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
}

void ABallProjectile::Destroyed()
{
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

	if (!OtherActor->Implements<UTeamInterface>())
	{
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
		return;
	}
	else {
		if (OtherActor->Implements<UHealthInterface>())
		{
			IHealthInterface::Execute_Damage(OtherActor, Damage);
		}
		else
		{
			Destroy();
			return;
		}
		Destroy();
	}
}
