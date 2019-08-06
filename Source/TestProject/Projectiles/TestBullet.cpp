// Fill out your copyright notice in the Description page of Project Settings.

#include "TestBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Perception/AISense_Damage.h"
#include "Engine.h"

#include "Player/EnemyPlayer.h"

ATestBullet::ATestBullet()
{
	
	// Tick will be called every frame to check for the range
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<class UCapsuleComponent>(TEXT("ProjectileCollisionComponent"));
	CollisionComponent->InitCapsuleSize(0.3f, 0.5f);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// So that player doesn't step on the bullet
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	// Collision Capsule will be the root component
	RootComponent = CollisionComponent;

	// Movement
	ProjectileMovementComponent = CreateDefaultSubobject<class UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = 10000.0f;
	ProjectileMovementComponent->MaxSpeed = 10000.0f;
	ProjectileMovementComponent->bShouldBounce = false;

	OnHitDamage = 250.f;

}

void ATestBullet::Fire(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void ATestBullet::BeginPlay()
{
	Super::BeginPlay();

	// Add the delegate to handle the overlapping of the projectiles with the zombies (or other objects)
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATestBullet::OnOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATestBullet::OnOverlapEnd);

	// Add the delegate to handle the collision with objects in world
	CollisionComponent->OnComponentHit.AddDynamic(this, &ATestBullet::OnHit);
}

void ATestBullet::OnOverlap(class UPrimitiveComponent* OverlapComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	static int count = 0;
	AEnemyPlayer* Player = Cast<AEnemyPlayer>(OtherActor);
	if (Player)
	{
		count++;
		if (count % 4 == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Hit Player", true);
		}
	}
}

void ATestBullet::OnOverlapEnd(class UPrimitiveComponent* OverlapComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ATestBullet::OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	AEnemyPlayer* Player = Cast<AEnemyPlayer>(OtherActor);
	if (Player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Hit Player", true);
	}

	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(NormalImpulse, HitResult.ImpactPoint);
	}
}