// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BTTask_LookAround.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UBTTask_LookAround : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_LookAround();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& BehaviorComponent, uint8* NodeMemory);

	/* Radius for searching */
	UPROPERTY(EditAnywhere)
	float SearchRadius = 400.0f;
};
