// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestTypes.h"
#include "TestBullet.generated.h"

/**
 *
 */
UCLASS()
class TESTPROJECT_API ATestBullet : public AActor
{
	GENERATED_BODY()

public:
	/* Constructor */
	ATestBullet();

	/* To fire the bullet and set the range and the spawn location */
	void Fire(const FVector& ShootDirection);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/* Delegate to handle the overlap begin event of the projectile */
	UFUNCTION()
	void OnOverlap(class UPrimitiveComponent* OverlapComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);

	/* Delegate to handle the overlap end event of the projectile */
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlapComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/* Delegate to handle the hit event of the projectile */
	UFUNCTION()
	void OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	/* Damage dealt by the projectile on collision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float OnHitDamage;

private:
	/* Collision component of the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", META = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CollisionComponent;

	/* Movement component of the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", META = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementComponent;
};
