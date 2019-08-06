// Fill out your copyright notice in the Description page of Project Settings.

#include "TestWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "Engine.h"

#include "Projectiles/TestBullet.h"


// Sets default values
ATestWeapon::ATestWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*******************  Create the weapon mesh ****************/
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	WeaponMesh->bReceivesDecals = false;
	WeaponMesh->CastShadow = false;
	RootComponent = WeaponMesh;

	/******************* Create the muzzle location object ********/
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// Initialize the properties
	bWantsToReload = false;
	bWantsToFire = false;
	CurrentWeaponState = EWeaponState::IDLE;
	AmmoPerMag = 30;
	InitialMags = 10;
	MaxAmmo = 300;
	RateOfFire = 600;

	WeaponRange = 10000.0f;

	AttachPointOnEquip = "WeaponAttachPoint";
}

// Called when the game starts or when spawned
void ATestWeapon::BeginPlay()
{
	Super::BeginPlay();

	//Attach to the un-equipped position
	if (WeaponOwner)
	{
		WeaponMesh->AttachToComponent(WeaponOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPointOnEquip);
	}

	TimeBetweenShots = 60.0f / RateOfFire;
}

// Called every frame
void ATestWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Set the relative location
	if(MuzzleLocation)
		MuzzleLocation->SetRelativeLocation(MuzzleOffset);

	// Initialize the current ammo
	CurrentMagAmmo = AmmoPerMag;
	CurrentAmmo = InitialMags * AmmoPerMag;

	if (CurrentAmmo > MaxAmmo)
		CurrentAmmo = MaxAmmo;
}

void ATestWeapon::Destroyed()
{
	Super::Destroyed();

	StopWeaponFire();

	StopReload();

	GetWorldTimerManager().ClearAllTimersForObject(this);
}

bool ATestWeapon::CanFire() const
{
	return (CurrentWeaponState == EWeaponState::IDLE) || (CurrentWeaponState == EWeaponState::FIRING);
}

bool ATestWeapon::CanReload() const
{
	bool bIsInValidStateToReload = (CurrentWeaponState == EWeaponState::IDLE || CurrentWeaponState == EWeaponState::FIRING);
	bool bIsReloadNeeded = (CurrentMagAmmo >= 0 && CurrentMagAmmo < AmmoPerMag) && (CurrentAmmo > 0);
	return bIsInValidStateToReload && bIsReloadNeeded;
}

void ATestWeapon::StartFire()
{
	if (!bWantsToFire)
	{
		bWantsToFire = true;
	}

	CheckWeaponState();
}

void ATestWeapon::StopFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
		GetWorldTimerManager().ClearTimer(TimerHandle_AutomaticFire);

		CheckWeaponState();
	}
}

void ATestWeapon::StartReload()
{
	if (!bWantsToReload)
	{
		bWantsToReload = true;
		CheckWeaponState();
	}
}

void ATestWeapon::ReloadWeapon()
{
	//Check if the reload is even needed
	if (CurrentMagAmmo < AmmoPerMag)
	{
		//Check if the reload is possible
		if (CurrentAmmo > 0)
		{
			int32 AmountNeededToRefill = AmmoPerMag - CurrentMagAmmo;
			int32 AmountRefilled = (CurrentAmmo >= AmountNeededToRefill) ? AmountNeededToRefill : (AmountNeededToRefill - CurrentAmmo);
			CurrentMagAmmo += AmountRefilled;
			CurrentAmmo -= AmountRefilled;

			if (CurrentAmmo < 0)
				CurrentAmmo = 0;
		}
	}

	// Set the state after reloading
	bWantsToReload = false;

	CheckWeaponState();
}

void ATestWeapon::StopReload()
{
	if (bWantsToReload)
	{
		bWantsToReload = false;

		//Clear the timer so that queued reloading task is dequeued
		GetWorldTimerManager().ClearTimer(TimerHandle_ReloadWeapon);

		CheckWeaponState();
	}
}

void ATestWeapon::AddAmmo(const int32 Amount)
{
	CurrentAmmo += Amount;
	if (CurrentAmmo > MaxAmmo)
		CurrentAmmo = MaxAmmo;
}

void ATestWeapon::SetWeaponOwner(class ACharacter* NewOwner)
{
	if (NewOwner)
	{
		Instigator = NewOwner;
		WeaponOwner = NewOwner;
	}
}

void ATestWeapon::CheckWeaponState()
{
	if (bWantsToReload)
	{
		if (CanReload())
		{
			SetWeaponState(EWeaponState::RELOADING);
			ReloadWeapon();
		}
	}
	else if (bWantsToFire)
	{
		if (CanFire())
		{
			SetWeaponState(EWeaponState::FIRING);
		}
		else
		{
			SetWeaponState(EWeaponState::IDLE);
		}
	}
	else
	{
		SetWeaponState(EWeaponState::IDLE);
	}
}

void ATestWeapon::SetWeaponState(EWeaponState NewState)
{
	EWeaponState PrevState = CurrentWeaponState;
	CurrentWeaponState = NewState;

	if (PrevState == EWeaponState::FIRING && (NewState == EWeaponState::IDLE || NewState == EWeaponState::RELOADING))
		StopWeaponFire();

	else if ((PrevState == EWeaponState::IDLE || PrevState == EWeaponState::RELOADING || PrevState == EWeaponState::FIRING) && NewState == EWeaponState::FIRING)
		StartWeaponFire();
}

void ATestWeapon::StartWeaponFire()
{
	//Check if can fire weapon (Have bullets)
	if (CurrentMagAmmo > 0)
	{
		//Get the location where the bullet will originate and the direction of fire
		FRotator SpawnRotation;
		FVector SpawnLocation;

		//Get the location where to spawn the bullet
		if (WeaponOwner)
		{
			WeaponOwner->GetController()->GetActorEyesViewPoint(SpawnLocation, SpawnRotation);
			SpawnLocation = MuzzleLocation->GetComponentLocation() + SpawnRotation.RotateVector(MuzzleOffset);
		}

		const FVector EulerAngles = GetRandomSpread(SpawnRotation);

		// Add spread to the bullet based on the current action of the player
		SpawnRotation.Add(EulerAngles.Y, EulerAngles.Z, EulerAngles.X);

		// Set the spawn parameters
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		SpawnParams.Instigator = WeaponOwner;

		ATestBullet* Bullet = GetWorld()->SpawnActor<ATestBullet>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);

		// If the bullet was successfully created, then fire it
		if (Bullet)
		{
			// Get the velocity vector for the bullet
			FVector ShootDirection = SpawnRotation.Vector();
			ShootDirection.Normalize();

			Bullet->Fire(ShootDirection);

			//Consume Ammo
			CurrentMagAmmo--;

			// Set the timer to fire the next bullet
			GetWorldTimerManager().SetTimer(TimerHandle_AutomaticFire, this, &ATestWeapon::StartFire, TimeBetweenShots, false);
		}
	}
	//If can not fire, check if can reload
	else if (CanReload())
	{
		// Stop the firing before reloading
		StopWeaponFire();

		//Start Reload
		StartReload();
	}
	//Else Set State to IDLE
	else
	{
		//Notify can't fire
		SetWeaponState(EWeaponState::IDLE);
	}
}

void ATestWeapon::StopWeaponFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;

		// Notify the Instigator to stop simulating fire
		//WeaponOwner->StopFire();

		CheckWeaponState();
	}
}

const FVector ATestWeapon::GetRandomSpread(const FRotator& Rotation) const
{
	//// Current Spread Radius
	//float CurrentSpreadRadius;

	////Get the current stance of the weapon owner
	//EPlayerStance CurrentStance = WeaponOwner->GetCurrentStance();

	////To generate random euler angles
	//static FRandomStream RandomStream;

	////Get the spread
	//if (WeaponOwner)
	//{
	//	if (WeaponOwner->IsMoving())
	//	{
	//		CurrentSpreadRadius = MovingFireSpreadRadius;
	//	}
	//	else if (WeaponOwner->IsTargeting() && CurrentStance == EPlayerStance::Crouch)
	//	{
	//		CurrentSpreadRadius = FMath::Max(TargetingSpreadRadius, CrouchFireSpreadRadius) / FMath::Min(TargetingSpreadRadius, CrouchFireSpreadRadius);
	//	}
	//	else if (WeaponOwner->IsTargeting() && CurrentStance == EPlayerStance::Prone)
	//	{
	//		CurrentSpreadRadius = FMath::Max(TargetingSpreadRadius, ProneFireSpreadRadius) / FMath::Min(TargetingSpreadRadius, ProneFireSpreadRadius);
	//	}
	//	else if (WeaponOwner->IsTargeting())
	//	{
	//		CurrentSpreadRadius = TargetingSpreadRadius;
	//	}
	//	else if (CurrentStance == EPlayerStance::Crouch)
	//	{
	//		CurrentSpreadRadius = CrouchFireSpreadRadius;
	//	}
	//	else if (CurrentStance == EPlayerStance::Prone)
	//	{
	//		CurrentSpreadRadius = ProneFireSpreadRadius;
	//	}
	//	else
	//	{
	//		CurrentSpreadRadius = FiringSpreadRadius;
	//	}

	//	//Get the Spread angle (Triangle formed by the radius of the circle at the range, the range the line from origin to circumference of the circle
	//	const float SpreadAngle = FMath::RadiansToDegrees(FMath::Atan2(CurrentSpreadRadius, WeaponRange / 50.0f));

	//	//Get Random Euler Angles
	//	float EulerAngleX = RandomStream.FRandRange(-SpreadAngle, SpreadAngle);
	//	float EulerAngleY = RandomStream.FRandRange(-SpreadAngle, SpreadAngle);
	//	float EulerAngleZ = RandomStream.FRandRange(-SpreadAngle, SpreadAngle);

	//	return FVector(EulerAngleX, EulerAngleY, EulerAngleZ);
	//}

	return FVector::ZeroVector;
}