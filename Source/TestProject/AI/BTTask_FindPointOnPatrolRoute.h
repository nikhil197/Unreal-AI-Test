// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPointOnPatrolRoute.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UBTTask_FindPointOnPatrolRoute : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BehaviorComponent, uint8* NodeMemory) override;

private:
	/* To keep track of the current index into the spline points */
	int CurrentPatrolPointIndex = 0;
};
