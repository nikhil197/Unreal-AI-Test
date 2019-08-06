// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyPlayer.generated.h"

UCLASS()
class TESTPROJECT_API AEnemyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Returns the current health of the player */
	inline float GetCurrentHealth() const { return HitPoints; }

	/* Returns the max health of the player */
	inline float GetMaxHealth() const { return MaxHitPoints; }

protected:
	/* Current Hit points of the player */
	float HitPoints;

	/* Max Hit points of the player */
	float MaxHitPoints;

private:
	void MoveForward(float Amount);

	void MoveRight(float Amount);

	void MakeSound();

};
