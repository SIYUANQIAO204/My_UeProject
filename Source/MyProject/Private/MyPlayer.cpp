// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/HealthComponent.h"
#include "Component/MyShootingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyCpp/MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/LuaSubsystem.h"
#include "AimComponent.h"
#include "Components/ArrowComponent.h"
#include "Component/CrosshairComponent.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
	//CameraBoom->SetRelativeRotation(FRotator(-80, 0.f, 0.f));
	CameraBoom->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f)); // Rotate the arm to look down at the character
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//避免角色旋转时相机也旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	CameraBoom->bInheritPitch = false;
	CameraBoom->bUsePawnControlRotation = false;
	FollowCamera->bUsePawnControlRotation = false;
	//角色随移动转向
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HP"));

	ShootingComponent = CreateDefaultSubobject<UMyShootingComponent>(TEXT("ShootingComponent"));

	AimComponent = CreateDefaultSubobject<UAimComponent>(TEXT("AimComponent"));

	MuzzleLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(GetMesh()); // Attach to the character's mesh at the "MuzzleSocket" socket
	MuzzleLocation->SetRelativeLocation(FVector(40.0f, 0.0f, 60.0f)); // Adjust as needed to position the muzzle correctly
	MuzzleLocation->SetRelativeRotation(FRotator::ZeroRotator); // Adjust as needed to orient the muzzle correctly

	CrosshairComponent = CreateDefaultSubobject<UCrosshairComponent>(TEXT("CrosshairComponent"));

}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	AimDirection = GetMuzzleForwardVector();
	if (const ULocalPlayer* Player = (GEngine && GetWorld()) ? GEngine->GetFirstGamePlayer(GetWorld()) : nullptr) 
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player);
		if (DefaultMapping)
		{
			Subsystem->AddMappingContext(DefaultMapping, 0);

		}
	}
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->UpdateHealthWidget(HealthComponent->GetHealthPercentage());
	}
	ShootingComponent->SetAimLocation(GetAimPoint());
}

void AMyPlayer::Move(const FInputActionValue& value)
{
	FVector2D MovementVector = value.Get<FVector2D>();
	if (Controller) 
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void AMyPlayer::Look(const FInputActionValue& value)
{
	FVector2D LookAxisVector = value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMyPlayer::OnAimPressed()
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController());
	if (!PlayerController) return;
	PlayerController->SetAiming(true);
	bIsAiming = true;
	//AimComponent->SetIsAiming(true);

	// 保存原始设置
	OriginalCameraBoomLocation = CameraBoom->GetRelativeLocation();
	OriginalCameraBoomLength = CameraBoom->TargetArmLength;

	// 切换到越肩视角
	CameraBoom->SetRelativeLocation(AimOffset);
	CameraBoom->TargetArmLength = AimCameraBoomLength;
	CameraBoom->bEnableCameraLag = false; // 禁用相机延迟以获得精确控制

	// 禁用controller对camera旋转的影响（瞄准时camera不随控制器旋转）
	FollowCamera->bUsePawnControlRotation = false;
}

void AMyPlayer::OnAimReleased()
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController());
	if (!PlayerController) return;
	PlayerController->SetAiming(false);
	bIsAiming = false;
	//AimComponent->SetIsAiming(false);

	// 恢复原始设置
	CameraBoom->SetRelativeLocation(OriginalCameraBoomLocation);
	CameraBoom->TargetArmLength = OriginalCameraBoomLength;
	CameraBoom->bEnableCameraLag = true; // 恢复相机延迟

	// 恢复controller对camera旋转的影响
	FollowCamera->bUsePawnControlRotation = true;
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ShootingComponent->SetAimLocation(GetAimPoint());
	if (bIsAiming)
	{
		CrosshairComponent->SetAimPoint(GetAimPoint());
		CrosshairComponent->UpdateScreenPosition(DeltaTime);
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->UpdateCrosshairScreenPoint(CrosshairComponent ->GetCurrentScreenPosition());
		}
	}
}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent Called"));
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		FVector SpawnLocation = GetMuzzleLocation();
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayer::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayer::Move);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AMyPlayer::OnShootPressed);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, ShootingComponent.Get(), &UMyShootingComponent::StopShooting);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AMyPlayer::OnAimPressed);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMyPlayer::OnAimReleased);
	}
}

void AMyPlayer::OnShootPressed()
{
	ShootingComponent->StartShooting(GetMuzzleLocation());
}

FVector AMyPlayer::GetMuzzleLocation() const
{
	return MuzzleLocation ? MuzzleLocation->GetComponentLocation() : GetActorLocation();
}

FVector AMyPlayer::GetMuzzleForwardVector() const
{
	return MuzzleLocation ? MuzzleLocation->GetForwardVector() : GetActorForwardVector();
}

FVector AMyPlayer::GetCameraLocation() const
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PC->PlayerCameraManager)
	{
		return PC->PlayerCameraManager->GetCameraLocation();
	}
	return FVector::ZeroVector;
}

FVector AMyPlayer::GetCameraForwardVector() const
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PC->PlayerCameraManager)
	{
		return PC->PlayerCameraManager->GetCameraRotation().Vector();
	}

	return FVector::ZeroVector;
}

FVector AMyPlayer::GetAimPoint() const
{
	FVector Start;
	FVector End;
	if (bIsAiming) {
		Start = GetCameraLocation();
		End = Start + GetCameraForwardVector() * 10000.0f; // Trace 10,000 units ahead
	}
	else {
		Start = GetMuzzleLocation();
		End = Start + GetMuzzleForwardVector() * 10000.0f; // Trace 10,000 units ahead
	}
	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (bHit)
	{
		return HitResult.Location;
	}
	return End;
}

FRotator AMyPlayer::GetCameraRotation() const
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PC->PlayerCameraManager)
	{
		return PC->PlayerCameraManager->GetCameraRotation();
	}
	return FRotator::ZeroRotator;
}

void AMyPlayer::Death_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Player has died!"));
	//UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->CreateRestartWidget();
	}
}

void AMyPlayer::Damage_Implementation(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Player has taken damage!"));
	if(HealthComponent)
	{
		HealthComponent->TakeDamage(DamageAmount);
	}
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->UpdateHealthWidget(HealthComponent->GetHealthPercentage());
	}
}

ETeam AMyPlayer::GetTeam_Implementation() const
{
	return Team;
}

void AMyPlayer::CallLua()
{
	/*if (UGameInstance* GameInstance = GetGameInstance())
	{
		ULuaSubsystem* LuaSubsystem = GameInstance->GetSubsystem<ULuaSubsystem>();
		if(LuaSubsystem)
		{
			LuaSubsystem->DoFile(TEXT("Main.lua"));
			LuaSubsystem->CallFunction(TEXT("Main"),{});

		}
	}*/
}