// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_MoveInFiringRange.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UBTService_MoveInFiringRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_MoveInFiringRange(const FObjectInitializer& ObjectInitializer);

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool RunLineTraceToActor(const AActor* Querier, const AActor* Target, const FVector& TraceStartLocation, const FVector& TraceEndLocation);

	bool LiesInFiringRange(const FVector& VantagePoint, const FVector& TargetLocation, float WeaponRange);

protected:
	/* Blackboard key that should be updated by the service */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector KeyToUpdate;
};
