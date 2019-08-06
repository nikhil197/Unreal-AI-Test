// Fill out your copyright notice in the Description page of Project Settings.


#include "AISoldierCharacter.h"
#include "Components/SplineComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "Weapon/TestWeapon.h"
#include "AI/Controller/SoldierAIController.h"

// Sets default values
AAISoldierCharacter::AAISoldierCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = EAIState::PARTOLLING;
	AIControllerClass = ASoldierAIController::StaticClass();

	PatrolPath = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolPath"));
	PatrolPath->SetupAttachment(GetMesh());

	MaxHitPoints = 100.0f;
	HitPoints = MaxHitPoints;
}

void AAISoldierCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;

	Weapon = GetWorld()->SpawnActor<ATestWeapon>(WeaponClass, GetActorLocation(), GetActorRotation(), params);

	if (Weapon)
	{
		Weapon->SetWeaponOwner(this);
		//Weapon->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

// Called when the game starts or when spawned
void AAISoldierCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAISoldierCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAISoldierCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAISoldierCharacter::OnStartFire()
{
	if (!bWantsToFire && Weapon)
	{
		bWantsToFire = true;
		Weapon->StartFire();
	}
}

void AAISoldierCharacter::OnStopFire()
{
	if (bWantsToFire && Weapon)
	{
		bWantsToFire = false;
		Weapon->StopFire();
	}
}

void AAISoldierCharacter::OnReload()
{
	if (!bWantsToReload && Weapon)
	{
		bWantsToReload = true;
		Weapon->StartReload();
	}
}

void AAISoldierCharacter::OnStopReload()
{
	if (bWantsToReload && Weapon)
	{
		bWantsToReload = false;
		Weapon->StopReload();
	}
}