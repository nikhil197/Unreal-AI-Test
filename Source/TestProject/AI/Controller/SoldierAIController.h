// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TestTypes.h"
#include "Perception/AIPerceptionTypes.h"
#include "SoldierAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API ASoldierAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASoldierAIController();

	void SetAIState(EAIState NewState);

	EAIState GetAIState() const;

	void SetDestination(const FVector& Destination);

	FVector GetDestination() const;

	void SetTarget(AActor* Target);

	const class AActor* GetTarget() const;

	void SetLastKnownLocation(const FVector& Loc);

	FVector GetLastKnownLocation() const;

	bool DoesAINeedAmmo() const;

	bool DoesAINeedHealth() const;

private:
	//// AController Interface Begin
	virtual void OnPossess(class APawn* PossessedPawn) override;

	virtual void OnUnPossess() override;

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;
	//// AController Interface End

	/* Handles the search time over event */
	void OnSearchTimeOver();

	/* Handles the investigation time over event */
	void OnInvestigationTimeOver();

	/* Delegate to handle the target perception update event */
	UFUNCTION()
	void OnTargetPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY(BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception", META = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception", META = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Damage* DamageConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception", META = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight* SightConfig;

	/* Name for the Current AI State key in the blackboard */
	FName StateKeyName;

	/* Name for the current destination key in the blackboard */
	FName DestinationKeyName;

	/* Name for the Target key in the blackboard */
	FName TargetKeyName;

	/* Name for the last known location key in the blackboard */
	FName LastKnownLocKeyName;

	/* Name for the Need Ammo key in the blackboard */
	FName NeedAmmoKeyName;

	/* Name for the Need Health key in the blackboard */
	FName NeedHealthKeyName;

	/* Current sound source that is being investigated */
	ESoundSource::Source CurrentInvestigatedSource;

	/* Time for which the AI will search around the area to look for the enemy */
	float SearchingTime = 20.0f;

	/* Time for which the AI will investigate a stimulus */
	float InvestigationTime = 10.0f;

	/* Timer handle to call the delegate when search time is over */
	FTimerHandle TimerHandle_SearchTimeExpired;

	/* Timer handle to call the delegate when investigation time is over */
	FTimerHandle TimerHandle_InvestigationTimeExpired;
};
