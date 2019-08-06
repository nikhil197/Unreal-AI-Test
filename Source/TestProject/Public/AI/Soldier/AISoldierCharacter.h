// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestTypes.h"
#include "AISoldierCharacter.generated.h"

UCLASS()
class TESTPROJECT_API AAISoldierCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAISoldierCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Called after all components are initialised */
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnStartFire();

	void OnStopFire();

	void OnReload();

	void OnStopReload();

	/* Returns the behavior tree used for the soldier */
	inline class UBehaviorTree* GetBehavior() const { return BehaviorTree; }

	/* Returns the path followed by the soldier when patrolling */
	inline class USplineComponent* GetPatrolPath() const { return PatrolPath; }

	/* Returns the current AI State */
	inline EAIState GetCurrentAIState() const { return CurrentState; }

	/* Returns the current health of the soldier */
	inline float GetCurrentHealth() const { return HitPoints; }

	/* Returns the max health of the soldier */
	inline float GetMaxHealth() const { return MaxHitPoints; }

	/* Returns the current equipped weapon */
	inline const class ATestWeapon* GetCurrentWeapon() const { return Weapon; }

protected:

	/* Max Hit points of the Soldier */
	UPROPERTY(EditDefaultsOnly, Category = Attributes)
	float MaxHitPoints;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class ATestWeapon> WeaponClass;

private:
	/* Current Hit points of the AI Soldier */
	float HitPoints;

	/* Current state the AI is in */
	EAIState CurrentState;

	bool bWantsToFire : 1;

	bool bWantsToReload : 1;

	class ATestWeapon* Weapon;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	class USplineComponent* PatrolPath;

};
